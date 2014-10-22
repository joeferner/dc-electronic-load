#! /usr/bin/env python

import sys
import os
import mimetypes
import json

mimetypes.init()

inputPath = './flash-files'
outputFilename = './out.img'
flashFilesCFilename = './firmware/flashFiles.c'
offsetInOutputFile = 0
files = []
mimeTypes = {}

fout = open(outputFilename, 'wb')

def readdir(path):
  global offsetInOutputFile

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

      script = 'serve_flash_file'
      mimeType = mimetypes.guess_type(relFilename)[0]

      if mimeType == None:
        with open(filename, "r") as jsonFile:
          data = jsonFile.read()
          j = json.loads(data)
          script = j['script']
          fileSize = 0
      else:
        while True:
          data = fin.read(512)
          if len(data) == 0:
            break
          fout.write(data)

      mimeTypes[mimeType] = True

      files.append({ 
        'filename': relFilename,
        'contentType': mimeType,
        'size': fileSize, 
        'offset': offsetInOutputFile,
        'script': script
      })

      fin.close()
      offsetInOutputFile += fileSize
    
    elif os.path.isdir(filename):
      readdir(filename)

readdir(inputPath);

fout.close()

def mimeTypeToVariableName(mimeType):
  if mimeType == None:
    return 'mimetype_text_html'
  return 'mimetype_' + mimeType.replace('/', '_')

fout = open(flashFilesCFilename, 'w')

fout.write("""
#include <stdlib.h>
#include "flashFiles.h"

""")

for mimeType in mimeTypes:
  if mimeType != None:
    fout.write('static const char ' + mimeTypeToVariableName(mimeType) + '[] = "' + mimeType + '";\n')

fout.write("""
  
struct flashFile flashFiles[] = {
""")

for f in files:
  fout.write('  { .filename = "/' + f['filename'] + '", .content_type = ' + mimeTypeToVariableName(f['contentType']) + ', .offset = ' + str(f['offset']) + ', .size = ' + str(f['size']) + ', .script = ' + f['script'] + ' },\n')
  
fout.write('  { .filename = NULL, .offset = 0, .size = 0 }\n')
fout.write('};\n')
  
fout.close()
