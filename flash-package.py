#! /usr/bin/env python

import sys
import os
import mimetypes

mimetypes.init()

inputPath = './flash-files'
outputFilename = './out.img'
flashFilesCFilename = './firmware/flashFiles.c'

files = []
mimeTypes = {}

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

      mimeType = mimetypes.guess_type(relFilename)[0]
      mimeTypes[mimeType] = True

      files.append({ 
        'filename': relFilename,
        'contentType': mimeType,
        'size': fileSize, 
        'offset': offset
      })

      fin.close()
      offset += fileSize
    
    elif os.path.isdir(filename):
      readdir(filename)

readdir(inputPath);

fout.close()

def mimeTypeToVariableName(mimeType):
  return 'mimetype_' + mimeType.replace('/', '_')

fout = open(flashFilesCFilename, 'w')

fout.write("""
#include <stdlib.h>
#include "flashFiles.h"

""")

for mimeType in mimeTypes:
  fout.write('static const char ' + mimeTypeToVariableName(mimeType) + '[] = "' + mimeType + '";\n')

fout.write("""
  
struct flashFile flashFiles[] = {
""")

for f in files:
  fout.write('  { .filename = "/' + f['filename'] + '", .content_type = ' + mimeTypeToVariableName(f['contentType']) + ', .offset = ' + str(f['offset']) + ', .size = ' + str(f['size']) + ', .script = serve_flash_file },\n')
  
fout.write('  { .filename = NULL, .offset = 0, .size = 0 }\n')
fout.write('};\n')
  
fout.close()
