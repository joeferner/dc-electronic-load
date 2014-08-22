#ifndef _ring_buffer_h_
#define _ring_buffer_h_

#include <stdint.h>
#include <stm32f10x_dma.h>

typedef struct _ring_buffer_u8 {
  uint8_t* storage;
  uint8_t* end;
  uint16_t size;
  volatile uint8_t* read;
  volatile uint8_t* write;
  volatile uint16_t available;
} ring_buffer_u8;

void ring_buffer_u8_init(ring_buffer_u8* ring, uint8_t* storage, uint16_t size);
uint16_t ring_buffer_u8_available(ring_buffer_u8* ring);
uint16_t ring_buffer_u8_free(ring_buffer_u8* ring);
void ring_buffer_u8_read(ring_buffer_u8* ring, uint8_t* buffer, uint16_t size);
uint8_t ring_buffer_u8_read_byte(ring_buffer_u8* ring);
void ring_buffer_u8_write(ring_buffer_u8* ring, const uint8_t* buffer, uint16_t size);
void ring_buffer_u8_write_byte(ring_buffer_u8* ring, uint8_t b);
uint16_t ring_buffer_u8_readline(ring_buffer_u8* ring, char* buffer, uint16_t size);
uint8_t ring_buffer_u8_peek(ring_buffer_u8* ring);
uint8_t ring_buffer_u8_peekn(ring_buffer_u8* ring, uint16_t i);

typedef struct _dma_ring_buffer {
  DMA_Channel_TypeDef* ch;
  uint8_t* buffer;
  uint8_t* bufferEnd;
  uint8_t* currentPos;
} dma_ring_buffer;

void dma_ring_buffer_init(
  dma_ring_buffer* ring,
  DMA_Channel_TypeDef* ch,
  uint8_t* buffer,
  uint16_t bufferSize);
uint16_t dma_ring_buffer_read(dma_ring_buffer* ring, uint8_t* buffer, uint16_t size);
uint16_t dma_ring_buffer_readline(dma_ring_buffer* ring, char* line, uint16_t size);

#endif
