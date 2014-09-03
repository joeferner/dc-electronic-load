PROJECT(dc-electronic-load)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENABLE_LANGUAGE(ASM)

IF(NOT STM32_USB_FS_DEVICE_LIB_DIR)
  SET(STM32_USB_FS_DEVICE_LIB_DIR "/opt/STM32_USB-FS-Device_Lib_V4.0.0")
  MESSAGE(STATUS "No STM32_USB_FS_DEVICE_LIB_DIR specified, using default: " ${STM32_USB_FS_DEVICE_LIB_DIR})
ENDIF()

SET(CMSIS_FIND_LIBS "cmsis_md")
SET(CMSIS_STARTUP_SOURCE "${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/CMSIS/Device/ST/STM32F10x/Source/Templates/gcc_ride7/startup_stm32f10x_md.s")
SET(STM32_CHIP_DEF "STM32F10X_MD")
SET(STACK_ADDRESS "0x20005000")
SET(FLASH_SIZE "128K")
SET(RAM_SIZE "20K")
SET(MIN_STACK_SIZE "0x200")
SET(MIN_HEAP_SIZE "0")
SET(EXT_RAM_SIZE "0K")
SET(FLASH_ORIGIN "0x08000000")
SET(RAM_ORIGIN "0x20000000")
SET(EXT_RAM_ORIGIN "0x60000000")

CONFIGURE_FILE(stm32_flash.md.in ${CMAKE_CURRENT_BINARY_DIR}/stm32_flash.md)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -D${STM32_CHIP_DEF} -I../ -I../contiki/")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -D${STM32_CHIP_DEF} -I../ -I../contiki/")
SET(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_BINARY_DIR}/stm32_flash.md ${CMAKE_EXE_LINKER_FLAGS}")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_STDPERIPH_DRIVER")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_STDPERIPH_DRIVER")

INCLUDE_DIRECTORIES(
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/CMSIS/Device/ST/STM32F10x/Include/
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/CMSIS/Include/
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/inc/
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32_USB-FS-Device_Driver/inc/
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET(PROJECT_SOURCES
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/misc.c
  system_stm32f10x.c

  contiki/net/uip.c
  contiki/net/uip_arp.c
  contiki/net/psock.c
  contiki/net/dhcpc.c
  contiki/net/tcpip.c
  contiki/net/resolv.c
  contiki/net/uip-udp-packet.c
  contiki/sys/timer.c
  contiki/sys/etimer.c
  contiki/sys/process.c
  contiki/sys/procinit.c
  contiki/lib/random.c

  main.c
  network.c
  interrupts.c
  debug.c
  stringUtils.c
  delay.c
  time.c
  ring_buffer.c
  enc28j60.c
  disp6800.c
  gfx.c
  encoder.c
  adc.c
  dac.c
  fan.c
)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${PROJECT_SOURCES} ${CMSIS_STARTUP_SOURCE})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}.elf ${CMSIS_LIBRARIES} ${StdPeriphLib_LIBRARIES})

ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.hex DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex)
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.bin DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.list DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJDUMP} -x -S ${CMAKE_PROJECT_NAME}.elf > ${CMAKE_PROJECT_NAME}.list)

