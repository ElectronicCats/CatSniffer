#
# Test connection CC1352P with sniffer firmware TI
# Andres Sabas @ Electronic Cats
# 27 de Agosto
#
import serial
import argparse
import os
import sys
import select
import time
import stat
import errno
import logging
import logging.handlers
import struct
import binascii

def CommandHandler_computeFcs(dataBuffer, length):
  result=0
  for i in length:
    result += dataBuffer + i
  return (result & 0xFF)

ser = serial.Serial(
    port='/dev/cu.usbmodemL430021X1',
    baudrate=921600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    xonxoff = False,
    rtscts = False,
    timeout = 0.1
    
)

while 1:
    #       UART interface in little endian format
    # StartFrame PacketInfo Length  Commandata   FCS   End
    #    4053      0x40      0x0000    0x40       0x40  0x4045
    #     2B        1B         2B      0-255B       1B    2B
    message_bytes = bytes.fromhex("4053400000404045") # CMD_PING
    #message_bytes = bytes.fromhex("4053450400610300802D4045") CMD_CFG_FREQUENCY
    ser.write(message_bytes)

    b = ser.read()
    hex_string = binascii.hexlify(b).decode('utf-8')
    print(hex_string)
    #time.sleep(1)