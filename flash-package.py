#! /usr/bin/env python

import sys
import os

inputPath = './flash-files'
outputFilename = './out.img'
flashFilesCFilename = './firmware/flashFiles.c'

files = []

fout = open(outputFilename, 'wb')

def readdir(path):
  offset = 0
  for f in os.listdir(path):
    filename = os.path.join(path, f)
    relFilename = os.path.relpath(filename, inputPath)
    if os.path.isfile(filename):
      print 'packaging: ' + relFilename
      
      fin = open(filename, "rb")

      fin.seek(0, 2)
      fileSize = fin.tell()
      print '  file size ' + str(fileSize)
      fin.seek(0)

      while True:
        data = fin.read(512)
        if len(data) == 0:
          break
        fout.write(data)

      files.append({ 'filename': relFilename, 'size': fileSize, 'offset': offset })

      fin.close()
      offset += fileSize
    
    elif os.path.isdir(filename):
      readdir(filename)

readdir(inputPath);

fout.close()

fout = open(flashFilesCFilename, 'w')

fout.write("""
#include <stdlib.h>
#include "flashFiles.h"

struct flashFile flashFiles[] = {
""")

for f in files:
  fout.write('  { .filename = "/' + f['filename'] + '", .offset = ' + str(f['offset']) + ', .size = ' + str(f['size']) + ' },\n')
  
fout.write('  { .filename = NULL, .offset = 0, .size = 0 }\n')
fout.write('};\n')
  
fout.close()
