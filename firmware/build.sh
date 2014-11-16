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

arm-none-eabi-size dc-electronic-load.elf | tee size.report

TEXT_SIZE=`cat size.report | tail -1 | cut -f 1 | tr -d ' '`
DATA_SIZE=`cat size.report | tail -1 | cut -f 2 | tr -d ' '`
BSS_SIZE=`cat size.report | tail -1 | cut -f 3 | tr -d ' '`

TEXT_SIZE_KB=$(echo "scale=1; ${TEXT_SIZE}/1024" | bc -l)
DATA_SIZE_KB=$(echo "scale=1; ${DATA_SIZE}/1024" | bc -l)
BSS_SIZE_KB=$(echo "scale=1; ${BSS_SIZE}/1024" | bc -l)

FLASH_TOTAL=$(echo "${TEXT_SIZE}+${DATA_SIZE}" | bc)
RAM_TOTAL=$(echo "${DATA_SIZE}+${BSS_SIZE}" | bc)
FLASH_REMAINING=$(echo "(128 * 1024)-${FLASH_TOTAL}" | bc)
RAM_REMAINING=$(echo "(20 * 1024)-${RAM_TOTAL}" | bc)

FLASH_TOTAL_KB=$(echo "scale=1; ${FLASH_TOTAL}/1024" | bc -l)
RAM_TOTAL_KB=$(echo "scale=1; ${RAM_TOTAL}/1024" | bc -l)
FLASH_REMAINING_KB=$(echo "scale=1; ${FLASH_REMAINING}/1024" | bc -l)
RAM_REMAINING_KB=$(echo "scale=1; ${RAM_REMAINING}/1024" | bc -l)

echo ""
echo "STM32 medium density ram = 20kB"
echo "STM32F103RBT6 flash = 128kB"
echo ""
echo "flash = text (${TEXT_SIZE_KB}kB) + data (${DATA_SIZE_KB}kB) = ${FLASH_TOTAL_KB}kB (remaining: ${FLASH_REMAINING_KB}kB)"
echo "  ram = data (${DATA_SIZE_KB}kB) +  bss (${BSS_SIZE_KB}kB) = ${RAM_TOTAL_KB}kB (remaining: ${RAM_REMAINING_KB}kB)"
echo ""
