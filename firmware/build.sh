#!/bin/bash -e

GIT_HASH=`git rev-parse HEAD`

cat > ./version.h <<EOL
#ifndef _VERSION_H_
#define _VERSION_H_

#define GIT_HASH "${GIT_HASH}"

#endif
EOL

mkdir -p build
cd build
rm dc-electronic-load.elf || echo "Cannot remove. dc-electronic-load.elf not build?"
make dc-electronic-load.bin && \
make dc-electronic-load.list

echo ""
echo flash = text + data
echo ram = data + bss
echo STM32 medium density ram = 20kB
echo ""

arm-none-eabi-size dc-electronic-load.elf
