#!/bin/bash -e

mkdir -p build
cd build
rm dc-electronic-load.elf || echo "Cannot remove. dc-electronic-load.elf not build?"
make dc-electronic-load.bin && \
make dc-electronic-load.list
