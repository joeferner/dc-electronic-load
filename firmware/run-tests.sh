#!/bin/bash -e

gcc -o build/test \
	-DSTM32F10X_MD \
	-I. \
	-Icontiki \
	-I/opt/STM32_USB-FS-Device_Lib_V4.0.0/Libraries/STM32F10x_StdPeriph_Driver/inc \
        -I/opt/STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x \
	-I/opt/STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/CM3/CoreSupport/ \
	tests/debug.c tests/stm32f10x_exti.c tests/stm32f10x_gpio.c tests/stm32f10x_nvic.c tests/stm32f10x_rcc.c \
	tests/test.c stringUtils.c encoder.c
build/test

