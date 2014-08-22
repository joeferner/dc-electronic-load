#!/bin/bash

arm-none-eabi-gdb -tui -ex "target extended-remote localhost:4242" build/dc-electronic-load.elf
