import serial
import time
import sys

import argparse
import binascii
import threading
import logging.handlers
import struct

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

    def __init__(self, port, callback):

        baudrate = 921600
        rts_cts = False
        
        stats['Captured'] = 0
        stats['Non-Frame'] = 0

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
        self.running = False
        self.thread.join()
        #self.dev.ctrl_transfer(CC2531.DIR_OUT, CC2531.SET_STOP)

    def isRunning(self):
        return self.running	


    def recv(self):

        while self.running:
            if self.serial_port.in_waiting > 0:
                bytesteam = self.serial_port.read(self.serial_port.in_waiting)
                print ("RECV>> %s" % binascii.hexlify(bytesteam))
            time.sleep(0.05)
            
            #if ret[0] == 0:
            #    packet = self.parse_packet(ret)
            #    if packet:
            #        self.callback(packet)   
                             

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

if __name__ == "__main__":

    def callback(packet):
        print("-"*30)
        print(packet)
        print("-"*30)
        
    sniffer = CC1352('/dev/ttyACM0', callback)
    #sniffer = CC1352(callback)
    
    #print(sniffer)
    #sniffer.startc()
    #sniffer.pingc()
    #time.sleep(2)
    #sniffer.stopc()
    
    sniffer.pingc()   
    sniffer.stopc()       
    sniffer.cfgphyc()        
    sniffer.cfgfreqc()
    sniffer.initiatorc()
    sniffer.startc()
    print ("start")
    time.sleep(10)
    sniffer.stop() 
    sniffer.close()
