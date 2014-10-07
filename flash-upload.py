#! /usr/bin/env python

import serial
import sys
from time import sleep

BLOCK_SIZE = 512

if len(sys.argv) != 3:
  print 'filename serialPort required'
  sys.exit(1)

fileName = sys.argv[1]
port = sys.argv[2]

print 'opening file ' + fileName
f = open(fileName, "rb")
print 'opening file out.bin for write'
fout = open('out.bin', "wb")

f.seek(0, 2)
fileSize = f.tell()
print 'file size ' + str(fileSize)
f.seek(0)

print 'opening port ' + port
ser = serial.Serial(port, 9600, timeout = 1)

def readUntilOK():
  while True:
    line = ser.readline().strip()
    if "+OK" in line:
      return line
    if len(line) > 0:
      print 'rx: ' + line

tx = '!FLASHCLEAR\n'
print 'tx: ' + tx.strip()
ser.write(tx)
readUntilOK()

offset = 0
while offset < fileSize:
  tx = '!FLASHWRITE ' + str(offset) + '/' + str(fileSize) + '\n'
  print 'tx: ' + tx.strip()
  ser.write(tx)
  print 'transmitting bytes'
  fileData = f.read(BLOCK_SIZE)
  ser.write(fileData)
  if len(fileData) < BLOCK_SIZE:
    ser.write(b'\xFF' * (BLOCK_SIZE - len(fileData)))
  print str(len(fileData)) + ' bytes written. waiting for ack'
  readUntilOK()
  print 'ack rx'
  offset += BLOCK_SIZE
  sleep(0.01)

offset = 0
while offset < fileSize:
  tx = '!FLASHREAD ' + str(offset) + '\n'
  print 'tx: ' + tx.strip()
  ser.write(tx)
  readUntilOK()
  data = ser.read(BLOCK_SIZE)
  fout.write(data)
  print 'rx length: ' + str(len(data))
  offset += BLOCK_SIZE

print 'closing port'
ser.close()
