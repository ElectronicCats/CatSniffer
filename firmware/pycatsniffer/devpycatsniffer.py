import serial
import argparse
import binascii
import errno
import io
import logging.handlers
import os
import select
import stat
import struct
import sys
import threading
import time

__version__ = '0.0.1'

defaults = {
    'hex_file': 'ccsniffpiper.hexdump',
    'out_fifo': '/tmp/ccsniffpiper',
    'pcap_file': 'ccsniffpiper.pcap',
    'debug_level': 'WARNING',
    'log_level': 'INFO',
    'log_file': 'ccsniffpiper.log',
    'channel': 11,
}

logger = logging.getLogger(__name__)
stats = {}

ping = bytearray([0x40, 0x53, 0x40, 0x00, 0x00, 0x40, 0x40, 0x45])
stop = bytearray([0x40, 0x53, 0x42, 0x00, 0x00, 0x42, 0x40, 0x45])
cfgphy = bytearray([0x40, 0x53, 0x47, 0x01, 0x00, 0x09, 0x51, 0x40, 0x45])
cfgfreq = bytearray([0x40, 0x53, 0x45, 0x04, 0x00, 0x62, 0x09, 0x00, 0x00, 0xb4, 0x40, 0x45])
initiator = bytearray([0x40, 0x53, 0x70, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x40, 0x45])
letsgo = bytearray([0x40, 0x53, 0x41, 0x00, 0x00, 0x41, 0x40, 0x45])


class Frame(object):
    PCAP_FRAME_HDR_FMT = '<LLLL'

    def __init__(self, macPDUByteArray, timestampBy32):
        self.__macPDUByteArray = macPDUByteArray
        self.timestampBy32 = timestampBy32
        self.timestampUsec = timestampBy32 / 32.0
        self.len = len(self.__macPDUByteArray)

        self.__pcap_hdr = self.__generate_frame_hdr()

        self.pcap = self.__pcap_hdr + self.__macPDUByteArray
        self.hex = ''.join('%02x ' % c
                           for c in self.__macPDUByteArray).rstrip()

    def __generate_frame_hdr(self):
        sec = int(self.timestampUsec / 1000000)
        usec = int(self.timestampUsec - sec)
        return struct.pack(Frame.PCAP_FRAME_HDR_FMT, sec, usec, self.len,
                           self.len)

    def get_pcap(self):
        return self.pcap

    def get_hex(self):
        return self.hex

    def get_timestamp(self):
        return self.timestampUsec


#####################################


class PCAPHelper:
    LINKTYPE_IEEE802_15_4_NOFCS = 230
    LINKTYPE_IEEE802_15_4 = 195
    MAGIC_NUMBER = 0xA1B2C3D4
    VERSION_MAJOR = 2
    VERSION_MINOR = 4
    THISZONE = 0
    SIGFIGS = 0
    SNAPLEN = 0xFFFF
    NETWORK = LINKTYPE_IEEE802_15_4

    PCAP_GLOBAL_HDR_FMT = '<LHHlLLL'

    @staticmethod
    def writeGlobalHeader():
        return struct.pack(PCAPHelper.PCAP_GLOBAL_HDR_FMT,
                           PCAPHelper.MAGIC_NUMBER, PCAPHelper.VERSION_MAJOR,
                           PCAPHelper.VERSION_MINOR, PCAPHelper.THISZONE,
                           PCAPHelper.SIGFIGS, PCAPHelper.SNAPLEN,
                           PCAPHelper.NETWORK)


class FifoHandler(object):
    def __init__(self, out_fifo):
        self.out_fifo = out_fifo
        self.of = None
        self.needs_pcap_hdr = True
        self.thread = None
        self.running = False
        stats['Piped'] = 0
        stats['Not Piped'] = 0
        self.__create_fifo()
        self.__start()

    def __del__(self):
        self.__stop()

    def __start(self):
        logger.debug("start FIFO watcher thread")
        self.running = True
        self.thread = threading.Thread(target=self.__fifo_watcher)
        self.thread.daemon = True
        self.thread.start()

    def __stop(self):
        logger.debug("stop FIFO watcher thread")
        self.running = False
        self.thread.join()

    def __fifo_watcher(self):
        while self.running:
            self.__open_fifo(keepalive=True)
            time.sleep(0.01)

    def __create_fifo(self):
        try:
            os.mkfifo(self.out_fifo)
            logger.info(f'Opened FIFO {self.out_fifo}')
        except OSError as e:
            if e.errno == errno.EEXIST:
                if stat.S_ISFIFO(os.stat(self.out_fifo).st_mode) is False:
                    logger.error(
                        'File {self.out_fifo} exists and is not a FIFO')
                    sys.exit(1)
                else:
                    logger.warning(f'FIFO {self.out_fifo} exists. Using it')
            else:
                raise

    def __open_fifo(self, keepalive=False):
        try:
            fd = os.open(self.out_fifo, os.O_NONBLOCK | os.O_WRONLY)
            self.of = os.fdopen(fd, 'wb')
        except OSError as e:
            if e.errno == errno.ENXIO:
                if not keepalive:
                    logger.warning('Remote end not reading')
                    stats['Not Piped'] += 1
                self.of = None
                self.needs_pcap_hdr = True
            else:
                raise

    def triggerNewGlobalHeader(self):
        self.needs_pcap_hdr = True

    def handle(self, data):
        if self.of is None:
            self.__open_fifo()

        if self.of is not None:
            try:
                if self.needs_pcap_hdr is True:
                    logger.info('Write global PCAP header')
                    self.of.write(PCAPHelper.writeGlobalHeader())
                    self.needs_pcap_hdr = False
                self.of.write(data.pcap)
                self.of.flush()
                logger.debug(f'Wrote a frame of size {data.len} bytes')
                stats['Piped'] += 1
            except IOError as e:
                if e.errno == errno.EPIPE:
                    logger.info('Remote end stopped reading')
                    stats['Not Piped'] += 1
                    self.of = None
                    self.needs_pcap_hdr = True
                else:
                    raise


#####################################
class PcapDumpHandler(object):
    def __init__(self, filename):
        self.filename = filename
        stats['Dumped to PCAP'] = 0

        try:
            self.of = open(self.filename, 'wb')
            self.of.write(PCAPHelper.writeGlobalHeader())
            logger.info(f'Dumping PCAP to {self.filename}')
        except IOError as e:
            self.of = None
            logger.warning(
                f'Error opening {self.filename} to save pcap. Skipping')
            logger.warning(f'The error was: {e.args}')

    def handle(self, frame):
        if self.of is None:
            return
        self.of.write(frame.get_pcap())
        self.of.flush()
        logger.info(
            f'PcapDumpHandler: Dumped a frame of size {frame.len} bytes')
        stats['Dumped to PCAP'] += 1


class HexdumpHandler(object):
    def __init__(self, filename):
        self.filename = filename
        stats['Dumped as Hex'] = 0
        try:
            self.of = open(self.filename, 'wb')
            logger.info(f'Dumping hex to {self.filename}')
        except IOError as e:
            logger.warning(
                f'Error opening {self.filename} for hex dumps. Skipping')
            logger.warning(f'The error was: {e.args}')
            self.of = None

    def handle(self, frame):
        if self.of is None:
            return

        try:
            # Prepend the original timestamp in big-endian format
            self.of.write(
                binascii.hexlify(
                    struct.pack(">I ", int(frame.get_timestamp() * 32))))
            #self.of.write(str(frame.get_timestamp()))
            self.of.write(bytes("  ", 'ascii'))
            #             self.of.write('0000 ')
            self.of.write(bytes(frame.get_hex(), 'ascii'))
            self.of.write(bytes('\n', 'ascii'))
            self.of.flush()
            stats['Dumped as Hex'] += 1
            logger.info(
                f'HexdumpHandler: Dumped a frame of size {frame.len} bytes')
        except IOError as e:
            logger.warning(
                f'Error writing hex to {self.of} for hex dumps. Skipping')
            logger.warning(f'The error was: {e.args}')




class CC1352:

    DEFAULT_CHANNEL = 0x0B  # 11

    DATA_EP = 0x83
    DATA_TIMEOUT = 2500

    DIR_OUT = 0x40
    DIR_IN = 0xc0

    GET_IDENT = 0xc0
    SET_POWER = 0xc5
    GET_POWER = 0xc6

    SET_START = 0xd0  # bulk in starts
    SET_STOP = 0xd1  # bulk in stops
    SET_CHAN = 0xd2  # 0x0d (idx 0) + data)0x00 (idx 1)

    HEARTBEAT_FRAME = 0x01
    COMMAND_FRAME = 0x00
    
    BYTE_STREAM = 0

    def __init__(self, port, callback, channel=DEFAULT_CHANNEL):

        baudrate = 921600
        rts_cts = False
        
        stats['Captured'] = 0
        stats['Non-Frame'] = 0

        self.serial_port = None
        self.channel = channel
        self.callback = callback
        self.thread = None
        self.running = False
        
        try:
            #self.serial_port = serial.Serial(port, baudrate, 8, 'N', 1, timeout=1)
            self.serial_port = serial.Serial(port = port,
                                      baudrate = baudrate,
                                      bytesize = serial.EIGHTBITS,
                                      parity = serial.PARITY_NONE,
                                      stopbits = serial.STOPBITS_ONE,
                                      xonxoff = False,
                                      rtscts = rts_cts,
                                      timeout = 0.1)
            self.serial_port.flushInput()
            self.serial_port.flushOutput()
        except (serial.SerialException, ValueError, IOError, OSError) as e:
            logger.error('Error opening port: %s' % (port,))
            logger.error('The error was: %s' % (e.args,))
            sys.exit(1)
        logger.info('Serial port %s opened' % (self.serial_port.name))
        
    def close(self):
        self.serial_port.close()

    def pingc(self):
        self.serial_port.write(ping)
        
    def stopc(self):
        self.serial_port.write(stop) 
        
    def cfgphyc(self):
        self.serial_port.write(cfgphy)
        
    def cfgfreqc(self):
        self.serial_port.write(cfgfreq)

    def initiatorc(self):
        self.serial_port.write(initiator)        

    #def startc(self):
    #    self.serial_port.write(letsgo)
                               

    def startc(self):
        # start sniffing
        self.running = True
        #self.dev.ctrl_transfer(CC2531.DIR_OUT, CC2531.SET_START)
        
        self.serial_port.write(letsgo)
        
        self.thread = threading.Thread(target=self.recv)
        #self.thread.daemon = True
        self.thread.start()

    def stop(self):
        # end sniffing
        self.serial_port.write(stop) 
        self.running = False
        self.thread.join()
        #self.dev.ctrl_transfer(CC2531.DIR_OUT, CC2531.SET_STOP)

    def isRunning(self):
        return self.running	


    def recv(self):
	
        while self.running:
            if self.serial_port.in_waiting > 0:
                bytestream = self.serial_port.read(self.serial_port.in_waiting)
                
#        print ("RECV>> %s" % binascii.hexlify(bytestream))

            time.sleep(0.5)
            start_index = 0
            
            while True:
                # Find the index of the next occurrence of 0x40 0x53
                start_index = bytestream.find(b'\x40\x53', start_index)
                # If not found, break out of the loop
                if start_index == -1:
                    break
                # Find the index of the next occurrence of 0x40 0x45 after the start index
                end_index = bytestream.find(b'\x40\x45', start_index)
                # If not found, break out of the loop
                if end_index == -1:
                    break
                # Get the substream between start_index and end_index
                substream = bytestream[start_index:end_index+2]
                
                # Do something with the substream
                #print(substream)

                print ("SUBSRECV>> %s" % binascii.hexlify(substream))

                if len(substream) >= 3:
                    (cmd, cmdLen) = struct.unpack_from("<BH", substream)
                    payload = substream[3:]
                    if len(payload) == cmdLen:
                        # buffer contains the correct number of bytes
                        if CC1352.COMMAND_FRAME == cmd:
                            logger.info(f'Read a frame of size {cmdLen}')
                            stats['Captured'] += 1
                            (timestamp, pktLen) = struct.unpack_from("<IB", payload)
                            frame = payload[5:]

                            if len(frame) == pktLen:
                                self.callback(timestamp, frame.tobytes())
                            else:
                                logger.warning(
                                    f'Received a frame with incorrect length, pktLen:{pktLen}, len(frame):{len(frame)}'
                                )
                else:
                    logger.warning(
                        'Received a command response with unknown code - CMD:{:02x} byte:{}'
                        .format(cmd, substream))

                #packet = self.parse_packet(substream)
                #if packet:
                    #print("HELL O WORLD!")
                #    self.callback(packet)

                # Set the start index to end_index + 2 (to skip over the 0x40 0x45 bytes)
                start_index = end_index + 2
            
            
            #if bytestream[0:2] == bytes([0x40, 0x53]):
            #    packet = self.parse_packet(bytestream)
                #if packet:
                #    self.callback(packet)   
                             

    def set_channel(self, channel):
        was_running = self.running

        if channel >= 11 and channel <= 26:
            if self.running:
                self.stop()

            self.channel = channel

            # set channel command
            #self.dev.ctrl_transfer(CC2531.DIR_OUT, CC2531.SET_CHAN, 0, 0,
            #                       [channel])
            #self.dev.ctrl_transfer(CC2531.DIR_OUT, CC2531.SET_CHAN, 0, 1,
            #                       [0x00])

            self.get_channel()

            if was_running:
                self.start()

        else:
            raise ValueError("Channel must be between 11 and 26")

    def get_channel(self):
        return self.channel
        
    def parse_packet(self, packet):

        #print("***HELL***")

        packetlen = packet[3:5]

        #if len(packet) - 3 != packetlen:
        #    return None

        # unknown header produced by the radio chip
        header = packet[0:2]

        # the data in the payload
        payload = packet[11:-4]

        # length of the payload
        #payloadlen = packet[7] - 2 # without fcs

        #if len(payload) != payloadlen:
        #    return None

        # current time
        timestamp = time.gmtime()

        # used to derive other values
        fcs1, fcs2 = packet[-4:-2]

        # rssi is the signed value at fcs1
        rssi    = (fcs1 + 2**7) % 2**8 - 2**7  - 73

        # crc ok is the 7th bit in fcs2
        crc_ok  = fcs2 & (1 << 7) > 0

        # correlation value is the unsigned 0th-6th bit in fcs2
        corr    = fcs2 & 0x7f

        return Packet(timestamp, self.channel, header, payload, rssi, crc_ok, corr)
        

    def __repr__(self):

        if self.dev:
            return "%s <Channel: %d>" % (self.name, self.channel)
        else:
            return "Not connected"

class Packet:

    def __init__(self, timestamp, channel, header, payload, rssi, crc_ok, correlation):
        self.timestamp = timestamp
        self.channel = channel
        self.header = header
        self.payload = payload
        self.rssi = rssi
        self.crc_ok = crc_ok
        self.correlation = correlation

    def __repr__(self):
        
        ret = []
        ret.append("Channel:     %d" % self.channel)
        ret.append("Timestamp:   %s" % time.strftime("%H:%M:%S", self.timestamp))
        ret.append("Header:      %s" % binascii.hexlify(self.header))
        ret.append("RSSI:        %d" % self.rssi)
        ret.append("CRC OK:      %s" % self.crc_ok)
        ret.append("Correlation: %d" % self.correlation)
        ret.append("Payload:     %s" % binascii.hexlify(self.payload))

        return "\n".join(ret)

# if __name__ == "__main__":

#     def callback(packet):
#         print("-"*30)
#         print(packet)
#         print("-"*30)
        
#     sniffer = CC1352('/dev/ttyACM0', callback)
#     #sniffer = CC1352(callback)
    
#     #print(sniffer)
#     #sniffer.startc()
#     #sniffer.pingc()
#     #time.sleep(2)
#     #sniffer.stopc()
    
#     sniffer.pingc()   
#     sniffer.stopc()       
#     sniffer.cfgphyc()        
#     sniffer.cfgfreqc()
#     sniffer.initiatorc()
#     sniffer.startc()
#     print ("start")
#     time.sleep(1)
#     sniffer.stop() 
#     sniffer.close()

def arg_parser():
    debug_choices = ('DEBUG', 'INFO', 'WARNING', 'ERROR')

    parser = argparse.ArgumentParser(add_help=False,
                                     description='Read IEEE802.15.4 frames \
    from a CC1352 packet sniffer device, convert them to pcap and pipe them \
    into wireshark over a FIFO pipe for online analysis. Frames \
    can also be saved in a file in hexdump and/or pcap format for offline \
    analysis.')

    in_group = parser.add_argument_group('Input Options')
    in_group.add_argument(
        '-c',
        '--channel',
        type=int,
        action='store',
        choices=list(range(11, 27)),
        default=defaults['channel'],
        help='Set the sniffer\'s CHANNEL. Valid range: 11-26. \
                                  (Default: %s)' % (defaults['channel'], ))
    out_group = parser.add_argument_group('Output Options')
    out_group.add_argument(
        '-f',
        '--fifo',
        action='store',
        default=defaults['out_fifo'],
        help='Set FIFO as the named pipe for sending to wireshark. \
                                   If argument is omitted and -o option is not specified \
                                   the capture will pipe to: %s' %
        (defaults['out_fifo'], ))
    out_group.add_argument(
        '-o',
        '--offline',
        action='store_true',
        default=False,
        help='Disables sending the capture to the named pipe.')
    out_group.add_argument('-x',
                           '--hex-file',
                           action='store',
                           nargs='?',
                           const=defaults['hex_file'],
                           default=False,
                           help='Save the capture (hexdump) in HEX_FILE. \
                                   If -x is specified but HEX_FILE is omitted, \
                                   %s will be used. If the argument is \
                                   omitted altogether, the capture will not \
                                   be saved.' % (defaults['hex_file'], ))
    out_group.add_argument('-p',
                           '--pcap-file',
                           action='store',
                           nargs='?',
                           const=defaults['pcap_file'],
                           default=False,
                           help='Save the capture (pcap format) in PCAP_FILE. \
                                   If -p is specified but PCAP_FILE is omitted, \
                                   %s will be used. If the argument is \
                                   omitted altogether, the capture will not \
                                   be saved.' % (defaults['pcap_file'], ))

    log_group = parser.add_argument_group('Verbosity and Logging')
    log_group.add_argument(
        '-d',
        '--headless',
        action='store_true',
        default=False,
        help='Run in non-interactive/headless mode, without \
                                   accepting user input. (Default Disabled)')
    log_group.add_argument('-D',
                           '--debug-level',
                           action='store',
                           choices=debug_choices,
                           default=defaults['debug_level'],
                           help='Print messages of severity DEBUG_LEVEL \
                                   or higher (Default %s)' %
                           (defaults['debug_level'], ))
    log_group.add_argument('-L',
                           '--log-file',
                           action='store',
                           nargs='?',
                           const=defaults['log_file'],
                           default=False,
                           help='Log output in LOG_FILE. If -L is specified \
                                   but LOG_FILE is omitted, %s will be used. \
                                   If the argument is omitted altogether, \
                                   logging will not take place at all.' %
                           (defaults['log_file'], ))
    log_group.add_argument('-l',
                           '--log-level',
                           action='store',
                           choices=debug_choices,
                           default=defaults['log_level'],
                           help='Log messages of severity LOG_LEVEL or \
                                   higher. Only makes sense if -L is also \
                                   specified (Default %s)' %
                           (defaults['log_level'], ))

    gen_group = parser.add_argument_group('General Options')
    gen_group.add_argument('-v',
                           '--version',
                           action='version',
                           version='ccsniffpiper v%s' % (__version__))
    gen_group.add_argument('-h',
                           '--help',
                           action='help',
                           help='Shows this message and exits')

    return parser.parse_args()


def dump_stats():
    s = io.StringIO()

    s.write('Frame Stats:\n')
    for k, v in list(stats.items()):
        s.write('%20s: %d\n' % (k, v))

    print((s.getvalue()))


def log_init():
    logger.setLevel(logging.DEBUG)
    ch = logging.StreamHandler()
    ch.setLevel(getattr(logging, args.debug_level))
    cf = logging.Formatter('%(message)s')
    ch.setFormatter(cf)
    logger.addHandler(ch)

    if args.log_file is not False:
        fh = logging.handlers.RotatingFileHandler(filename=args.log_file,
                                                  maxBytes=5000000)
        fh.setLevel(getattr(logging, args.log_level))
        ff = logging.Formatter('%(asctime)s - %(levelname)8s - %(message)s')
        fh.setFormatter(ff)
        logger.addHandler(fh)


if __name__ == '__main__':
    args = arg_parser()
    log_init()

    logger.info('Started logging')

    handlers = []

    def handlerDispatcher(timestamp, macPDU):
        """ Dispatches any received frames to all registered handlers
            timestamp -> The timestamp the frame was received, as reported by the sniffer device, in microseconds
            macPDU -> The 802.15.4 MAC-layer PDU, starting with the Frame Control Field (FCF)
        """
        if len(macPDU) > 0:
            frame = Frame(macPDU, timestamp)
            for h in handlers:
                h.handle(frame)

    if args.offline is not True:
        f = FifoHandler(out_fifo=args.fifo)
        handlers.append(f)
    if args.hex_file is not False:
        handlers.append(HexdumpHandler(args.hex_file))
    if args.pcap_file is not False:
        handlers.append(PcapDumpHandler(args.pcap_file))

    if args.headless is False:
        h = io.StringIO()
        h.write('Commands:\n')
        h.write('c: Print current RF Channel\n')
        h.write('n: Trigger new pcap header before the next frame\n')
        h.write('h,?: Print this message\n')
        h.write('[11,26]: Change RF channel\n')
        h.write('s: Start/stop the packet capture\n')
        h.write('q: Quit')
        h = h.getvalue()

        e = 'Unknown Command. Type h or ? for help'

        print(h)

    snifferDev = CC1352('/dev/ttyACM0', handlerDispatcher, args.channel)
    try:

        while 1:
            if args.headless is True:
                if not snifferDev.isRunning():
                    snifferDev.start()
                # block until terminated (Ctrl+C or killed)
                snifferDev.thread.join()
            else:
                try:
                    if select.select([
                            sys.stdin,
                    ], [], [], 10.0)[0]:
                        cmd = sys.stdin.readline().rstrip()
                        logger.debug(f'User input: "{cmd}"')
                        if cmd in ('h', '?'):
                            print(h)
                        elif cmd == 'c':
                            # We'll only ever see this if the user asked for it, so we are
                            # running interactive. Print away
                            print(
                                f'Sniffing in channel: {snifferDev.get_channel()}'
                            )
                        elif cmd == 'n':
                            f.triggerNewGlobalHeader()
                        elif cmd == 'q':
                            logger.info('User requested shutdown')
                            sys.exit(0)
                        elif cmd == 's':
                            if snifferDev.isRunning():
                                snifferDev.stop()
                            else:
                                snifferDev.start()
                        elif int(cmd) in range(11, 27):
                            snifferDev.set_channel(int(cmd))
                        else:
                            raise ValueError
#                    else:
#                        logger.debug('No user input')
                except select.error:
                    logger.warning('Error while trying to read stdin')
                except ValueError as e:
                    print(e)
                except UnboundLocalError:
                    # Raised by command 'n' when -o was specified at command line
                    pass

    except (KeyboardInterrupt, SystemExit):
        logger.info('Shutting down')
        if snifferDev.isRunning():
            snifferDev.stop()
        dump_stats()
        sys.exit(0)