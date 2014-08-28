#include <stddef.h>
#include "ring_buffer.h"

#ifndef min
#define min(a,b) ( ((a) < (b)) ? (a) : (b) )
#endif

void ring_buffer_u8_init(ring_buffer_u8* ring, uint8_t* storage, uint16_t size) {
  ring->storage = storage;
  ring->size = size;
  ring->end = ring->storage + ring->size;
  ring->read = ring->storage;
  ring->write = ring->storage;
  ring->available = 0;
}

uint16_t ring_buffer_u8_available(ring_buffer_u8* ring) {
  return ring->available;
}

uint16_t ring_buffer_u8_free(ring_buffer_u8* ring) {
  return ring->size - ring->available;
}

uint8_t ring_buffer_u8_read_byte(ring_buffer_u8* ring) {
  if (ring->available == 0) {
    return 0;
  }
  uint8_t ret = *ring->read++;
  ring->available--;
  if (ring->read >= ring->end) {
    ring->read = ring->storage;
  }
  return ret;
}

void ring_buffer_u8_read(ring_buffer_u8* ring, uint8_t* buffer, uint16_t size) {
  uint16_t i;

  // TODO can be optimized
  for (i = 0; i < size; i++) {
    buffer[i] = ring_buffer_u8_read_byte(ring);
  }
}

void ring_buffer_u8_write_byte(ring_buffer_u8* ring, uint8_t b) {
  if (ring->available >= ring->size) {
    ring_buffer_u8_read_byte(ring);
  }

  *ring->write = b;
  ring->write++;
  ring->available++;
  if (ring->write >= ring->end) {
    ring->write = ring->storage;
  }
}

void ring_buffer_u8_write(ring_buffer_u8* ring, const uint8_t* buffer, uint16_t size) {
  uint16_t i;

  // TODO can be optimized
  for (i = 0; i < size; i++) {
    ring_buffer_u8_write_byte(ring, buffer[i]);
  }
}

uint16_t ring_buffer_u8_readline(ring_buffer_u8* ring, char* buffer, uint16_t size) {
  uint8_t b;
  uint16_t i;
  for (i = 0; i < min(ring->available, size - 1); i++) {
    b = ring_buffer_u8_peekn(ring, i);
    if (b == '\n') {
      i++;
      ring_buffer_u8_read(ring, (uint8_t*) buffer, i);
      buffer[i] = '\0';
      return i;
    }
  }
  buffer[0] = '\0';
  return 0;
}

uint8_t ring_buffer_u8_peek(ring_buffer_u8* ring) {
  return ring_buffer_u8_peekn(ring, 0);
}

uint8_t ring_buffer_u8_peekn(ring_buffer_u8* ring, uint16_t i) {
  if (i >= ring->available) {
    return 0;
  }

  uint8_t* read = (uint8_t*)ring->read;
  uint8_t* p = read + i;
  if (p >= ring->end) {
    p -= ring->size;
  }
  return *p;
}

void dma_ring_buffer_init(
  dma_ring_buffer* ring,
  DMA_Channel_TypeDef* ch,
  uint8_t* buffer,
  uint16_t bufferSize) {
  ring->ch = ch;
  ring->buffer = ring->currentPos = buffer;
  ring->bufferEnd = ring->buffer + bufferSize;
}

uint16_t dma_ring_buffer_read(dma_ring_buffer* ring, uint8_t* buffer, uint16_t size) {
  uint16_t bufferSize = ring->bufferEnd - ring->buffer;
  uint16_t dataCounter = bufferSize - DMA_GetCurrDataCounter(ring->ch);
  uint8_t* dataEnd = ring->buffer + dataCounter;
  uint8_t* p = ring->currentPos;
  uint8_t ch;
  uint8_t* out = buffer;
  uint8_t* bufferEnd = buffer + size;

  // data counter wrapped so read to the end
  if (dataEnd < ring->currentPos) {
    while (p < ring->bufferEnd && out < bufferEnd) {
      ch = *p++;
      *out++ = ch;
    }
    p = ring->buffer;
  }

  // read from p to dataEnd
  while (p < dataEnd && out < bufferEnd) {
    ch = *p++;
    *out++ = ch;
  }

  ring->currentPos = p;
  return out - buffer;
}

uint16_t dma_ring_buffer_readline(dma_ring_buffer* ring, char* line, uint16_t size) {
  uint32_t bufferSize = ring->bufferEnd - ring->buffer;
  uint32_t dataCounter = bufferSize - DMA_GetCurrDataCounter(ring->ch);
  uint8_t* dataEnd = ring->buffer + dataCounter;
  uint8_t* p = ring->currentPos;
  char ch;
  char* out = line;
  char* lineEnd = line + size;

  // data counter wrapped so read to the end
  if (dataEnd < ring->currentPos) {
    while (p < ring->bufferEnd && out < lineEnd) {
      ch = *p++;
      *out++ = ch;
      if (ch == '\n') {
        *out = '\0';
        ring->currentPos = p;
        return out - line;
      }
    }
    p = ring->buffer;
  }

  // read from p to dataEnd
  while (p < dataEnd && out < lineEnd) {
    ch = *p++;
    *out++ = ch;
    if (ch == '\n') {
      *out = '\0';
      ring->currentPos = p;
      return out - line;
    }
  }

  return 0;
}
