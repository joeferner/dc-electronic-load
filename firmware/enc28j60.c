
#include "platform_config.h"

#ifdef NETWORK_ENABLED

#include "enc28j60.h"
#include "enc28j60_constants.h"
#include "delay.h"
#include "debug.h"
#include "time.h"
#include "contiki/net/uip_arp.h"
#include "contiki/sys/timer.h"
#include "contiki/net/tcpip.h"

#ifndef MIN
#define MIN(a,b) ( ((a) < (b)) ? (a) : (b) )
#endif

//#define ENC28J60_DEBUG

void _enc28j60_send();
uint8_t _enc28j60_tcp_output();
uint16_t _enc28j60_receivePacket(uint8_t* buffer, uint16_t bufferLength);

void _enc28j60_packetSend(uint8_t* packet1, uint16_t packet1Length, uint8_t* packet2, uint16_t packet2Length);
void _enc28j60_writeOp(uint8_t op, uint8_t address, uint8_t data);
void _enc28j60_writeRegPair(uint8_t address, uint16_t data);
void _enc28j60_writeReg(uint8_t address, uint8_t data);
void _enc28j60_phyWrite(uint8_t address, uint16_t data);
uint16_t _enc28j60_phyRead(uint8_t address);
void _enc28j60_setBank(uint8_t address);
uint8_t _enc28j60_readReg(uint8_t address);
uint8_t _enc28j60_readOp(uint8_t op, uint8_t address);
uint16_t _enc28j60_readOp16(uint8_t op, uint8_t address);
void _enc28j60_readBuffer(uint8_t* data, uint16_t len);
void _enc28j60_writeBuffer(uint8_t* data, uint16_t len);

struct timer _enc28j60_periodicTimer;
uint16_t _enc28j60_nextPacketPtr;
uint8_t _enc28j60_bank = 0xff;

void enc28j60_setup(struct uip_eth_addr* macAddress) {
  timer_set(&_enc28j60_periodicTimer, CLOCK_SECOND / 4);

  // perform system reset
  enc28j60_reset_assert();
  delay_ms(50);
  enc28j60_reset_deassert();
  delay_ms(1000);

  _enc28j60_writeOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
  delay_ms(50);

  // check CLKRDY bit to see if reset is complete
  // The CLKRDY does not work. See Rev. B4 Silicon Errata point. Just wait.
  while(1) {
    uint8_t r = _enc28j60_readReg(ESTAT);
    if(r & ESTAT_CLKRDY) {
      break;
    }
#ifdef ENC28J60_DEBUG
    debug_write("?ESTAT: 0x");
    debug_write_u8(r,16);
    debug_write_line("");
#endif
    delay_ms(100);
  }

  // do bank 0 stuff
  // initialize receive buffer
  // 16-bit transfers, must write low byte first
  // set receive buffer start address
  _enc28j60_nextPacketPtr = RXSTART_INIT;

  // Rx start
  _enc28j60_writeRegPair(ERXSTL, RXSTART_INIT);

  // set receive pointer address
  _enc28j60_writeRegPair(ERXRDPTL, RXSTART_INIT);

  // RX end
  _enc28j60_writeRegPair(ERXNDL, RXSTOP_INIT);

  // TX start
  _enc28j60_writeRegPair(ETXSTL, TXSTART_INIT);

  // TX end
  //_enc28j60_writeRegPair(ETXNDL, TXSTOP_INIT);

  // do bank 2 stuff
  // enable MAC receive
  // and bring MAC out of reset (writes 0x00 to MACON2)
  _enc28j60_writeRegPair(MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS);

  // bring MAC out of reset
  _enc28j60_writeRegPair(MACON2, 0x00);

  // enable automatic padding to 60bytes and CRC operations
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN);

  // set inter-frame gap (non-back-to-back)
  _enc28j60_writeRegPair(MAIPGL, 0x0C12);

  // set inter-frame gap (back-to-back)
  _enc28j60_writeReg(MABBIPG, 0x12);

  // Set the maximum packet size which the controller will accept
  // Do not send packets longer than MAX_FRAMELEN:
  _enc28j60_writeRegPair(MAMXFLL, MAX_FRAMELEN);

  // do bank 1 stuff, packet filter:
  // For broadcast packets we allow only ARP packtets
  // All other packets should be unicast only for our mac (MAADR)
  //
  // The pattern to match on is therefore
  // Type     ETH.DST
  // ARP      BROADCAST
  // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
  // in binary these poitions are:11 0000 0011 1111
  // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
  //TODO define specific pattern to receive dhcp-broadcast packages instead of setting ERFCON_BCEN!
  _enc28j60_writeReg(ERXFCON, 0);//ERXFCON_UCEN | ERXFCON_CRCEN | ERXFCON_PMEN | ERXFCON_BCEN);
  _enc28j60_writeRegPair(EPMM0, 0x303f);
  _enc28j60_writeRegPair(EPMCSL, 0xf7f9);

  // do bank 3 stuff
  // write MAC address
  // NOTE: MAC address in ENC28J60 is byte-backward
  _enc28j60_writeReg(MAADR5, macAddress->addr[0]);
  _enc28j60_writeReg(MAADR4, macAddress->addr[1]);
  _enc28j60_writeReg(MAADR3, macAddress->addr[2]);
  _enc28j60_writeReg(MAADR2, macAddress->addr[3]);
  _enc28j60_writeReg(MAADR1, macAddress->addr[4]);
  _enc28j60_writeReg(MAADR0, macAddress->addr[5]);

  // no loopback of transmitted frames
  _enc28j60_phyWrite(PHCON2, PHCON2_HDLDIS);

  // switch to bank 0
  _enc28j60_setBank(ECON1);

  // enable interrutps
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE | EIE_PKTIE);

  // enable packet reception
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

  // Configure leds
  _enc28j60_phyWrite(PHLCON, 0x476);

  enc28j60_debugDump();

  tcpip_set_outputfunc(_enc28j60_tcp_output);

  delay_ms(100);
}

uint8_t _enc28j60_tcp_output() {
  uip_arp_out();
  _enc28j60_send();
  return 0;
}

void enc28j60_tick() {
  uip_len = _enc28j60_receivePacket(((uint8_t*)uip_buf), UIP_BUFSIZE);
  if(uip_len > 0) {
    struct uip_eth_hdr* header = ((struct uip_eth_hdr *)&uip_buf[0]);
    uint16_t packetType = header->type;

#ifdef ENC28J60_DEBUG
    debug_write("?receivePacket: len: ");
    debug_write_u16(uip_len, 10);
    debug_write(", dest: ");
    debug_write_uip_eth_addr(&header->dest);
    debug_write(", src: ");
    debug_write_uip_eth_addr(&header->src);
    debug_write(", type: ");
    debug_write_u16(packetType, 10);
    debug_write_line("");
    for (int i = 0; i < uip_len; i++) {
      debug_write_u8(uip_buf[i],16);
      debug_write(" ");
    }
    debug_write_line("");
#endif

    if (packetType == UIP_HTONS(UIP_ETHTYPE_IP)) {
#ifdef ENC28J60_DEBUG
      debug_write_line("?readPacket type IP");
#endif
      uip_arp_ipin();
      uip_input();
      if (uip_len > 0) {
        uip_arp_out();
        _enc28j60_send();
      }
    } else if (packetType == UIP_HTONS(UIP_ETHTYPE_ARP)) {
#ifdef ENC28J60_DEBUG
      debug_write_line("?readPacket type ARP");
#endif
      uip_arp_arpin();
      if (uip_len > 0) {
        _enc28j60_send();
      }
    }
  }

  if (timer_expired(&_enc28j60_periodicTimer)) {
    timer_restart(&_enc28j60_periodicTimer);
    for (int i = 0; i < UIP_CONNS; i++) {
      uip_periodic(i);
      // If the above function invocation resulted in data that
      // should be sent out on the Enc28J60Network, the global variable
      // uip_len is set to a value > 0.
      if (uip_len > 0) {
        uip_arp_out();
        _enc28j60_send();
      }
    }

#if UIP_UDP
    for (int i = 0; i < UIP_UDP_CONNS; i++) {
      uip_udp_periodic(i);

      // If the above function invocation resulted in data that
      // should be sent out on the Enc28J60Network, the global variable
      // uip_len is set to a value > 0. */
      if (uip_len > 0) {
        uip_arp_out();
        _enc28j60_send();
      }
    }
#endif /* UIP_UDP */
  }
}

void enc28j60_debugDump() {
  debug_write("?RevID: 0x");
  debug_write_u8(_enc28j60_readReg(EREVID), 16);
  debug_write_line("");

  debug_write_line("?Cntrl: ECON1 ECON2 ESTAT  EIR  EIE");
  debug_write("?       ");
  debug_write_u8(_enc28j60_readReg(ECON1), 16);
  debug_write("    ");
  debug_write_u8(_enc28j60_readReg(ECON2), 16);
  debug_write("    ");
  debug_write_u8(_enc28j60_readReg(ESTAT), 16);
  debug_write("     ");
  debug_write_u8(_enc28j60_readReg(EIR), 16);
  debug_write("   ");
  debug_write_u8(_enc28j60_readReg(EIE), 16);
  debug_write_line("");

  debug_write_line("?MAC  : MACON1  MACON2  MACON3  MACON4  MAC-Address");
  debug_write("?       0x");
  debug_write_u8(_enc28j60_readReg(MACON1), 16);
  debug_write("    0x");
  debug_write_u8(_enc28j60_readReg(MACON2), 16);
  debug_write("    0x");
  debug_write_u8(_enc28j60_readReg(MACON3), 16);
  debug_write("    0x");
  debug_write_u8(_enc28j60_readReg(MACON4), 16);
  debug_write("    ");
  debug_write_u8(_enc28j60_readReg(MAADR5), 16);
  debug_write_u8(_enc28j60_readReg(MAADR4), 16);
  debug_write_u8(_enc28j60_readReg(MAADR3), 16);
  debug_write_u8(_enc28j60_readReg(MAADR2), 16);
  debug_write_u8(_enc28j60_readReg(MAADR1), 16);
  debug_write_u8(_enc28j60_readReg(MAADR0), 16);
  debug_write_line("");

  debug_write_line("?Rx   : ERXST  ERXND  ERXWRPT ERXRDPT ERXFCON EPKTCNT MAMXFL");
  debug_write("?       0x");
  debug_write_u8(_enc28j60_readReg(ERXSTH), 16);
  debug_write_u8(_enc28j60_readReg(ERXSTL), 16);
  debug_write(" 0x");
  debug_write_u8(_enc28j60_readReg(ERXNDH), 16);
  debug_write_u8(_enc28j60_readReg(ERXNDL), 16);
  debug_write("  0x");
  debug_write_u8(_enc28j60_readReg(ERXWRPTH), 16);
  debug_write_u8(_enc28j60_readReg(ERXWRPTL), 16);
  debug_write(" 0x");
  debug_write_u8(_enc28j60_readReg(ERXRDPTH), 16);
  debug_write_u8(_enc28j60_readReg(ERXRDPTL), 16);
  debug_write("   0x");
  debug_write_u8(_enc28j60_readReg(ERXFCON), 16);
  debug_write("    0x");
  debug_write_u8(_enc28j60_readReg(EPKTCNT), 16);
  debug_write("  0x");
  debug_write_u8(_enc28j60_readReg(MAMXFLH), 16);
  debug_write_u8(_enc28j60_readReg(MAMXFLL), 16);
  debug_write_line("");

  debug_write_line("?Tx   : ETXST  ETXND  MACLCON1 MACLCON2 MAPHSUP");
  debug_write("?       0x");
  debug_write_u8(_enc28j60_readReg(ETXSTH), 16);
  debug_write_u8(_enc28j60_readReg(ETXSTL), 16);
  debug_write(" 0x");
  debug_write_u8(_enc28j60_readReg(ETXNDH), 16);
  debug_write_u8(_enc28j60_readReg(ETXNDL), 16);
  debug_write("   0x");
  debug_write_u8(_enc28j60_readReg(MACLCON1), 16);
  debug_write("     0x");
  debug_write_u8(_enc28j60_readReg(MACLCON2), 16);
  debug_write("     0x");
  debug_write_u8(_enc28j60_readReg(MAPHSUP), 16);
  debug_write_line("");
}

uint16_t _enc28j60_receivePacket(uint8_t* buffer, uint16_t bufferLength) {
#ifdef ENC28J60_DEBUG
  uint16_t rxstat;
#endif
  uint16_t len;

  // check if a packet has been received and buffered
  //if( !(_enc28j60_readReg(EIR) & EIR_PKTIF) ){
  // The above does not work. See Rev. B4 Silicon Errata point 6.
  if (_enc28j60_readReg(EPKTCNT) == 0) {
    return 0;
  }

#ifdef ENC28J60_DEBUG
  debug_write("read from: ");
  debug_write_u16(_enc28j60_nextPacketPtr, 10);
  debug_write_line("");
#endif

  // Set the read pointer to the start of the received packet
  _enc28j60_writeRegPair(ERDPTL, _enc28j60_nextPacketPtr);

  // read the next packet pointer
  _enc28j60_nextPacketPtr = _enc28j60_readOp16(ENC28J60_READ_BUF_MEM, 0);
#ifdef ENC28J60_DEBUG
  debug_write("nextPacketPtr: ");
  debug_write_u16(_enc28j60_nextPacketPtr, 10);
  debug_write_line("");
#endif

  // read the packet length (see datasheet page 43)
  len = _enc28j60_readOp16(ENC28J60_READ_BUF_MEM, 0);
  len -= 4; //remove the CRC count

  len = MIN(len, bufferLength);

  // read the receive status (see datasheet page 43)
#ifdef ENC28J60_DEBUG
  rxstat = _enc28j60_readOp16(ENC28J60_READ_BUF_MEM, 0);
  debug_write("rxstat: ");
  debug_write_u16(rxstat, 10);
  debug_write_line("");
#else
  _enc28j60_readOp16(ENC28J60_READ_BUF_MEM, 0);
#endif

  _enc28j60_readBuffer(buffer, len);

  // Move the RX read pointer to the start of the next received packet
  // This frees the memory we just read out
  _enc28j60_writeRegPair(ERXRDPTL, _enc28j60_nextPacketPtr);

  // decrement the packet counter indicate we are done with this packet
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);

  return len;
}

void _enc28j60_send() {
  if(uip_len <= UIP_LLH_LEN + 40) {
    _enc28j60_packetSend((uint8_t *)uip_buf, uip_len, 0, 0);
  } else {
    _enc28j60_packetSend((uint8_t *)uip_buf, 54, (uint8_t*)uip_appdata, uip_len - UIP_LLH_LEN - 40);
  }
}

void _enc28j60_writeReg(uint8_t address, uint8_t data) {
  // set the bank
  _enc28j60_setBank(address);

  // do the write
  _enc28j60_writeOp(ENC28J60_WRITE_CTRL_REG, address, data);
}

void _enc28j60_writeRegPair(uint8_t address, uint16_t data) {
  // set the bank
  _enc28j60_setBank(address);

  // do the write
  _enc28j60_writeOp(ENC28J60_WRITE_CTRL_REG, address, (data & 0xFF));
  _enc28j60_writeOp(ENC28J60_WRITE_CTRL_REG, address + 1, (data) >> 8);
}

void _enc28j60_phyWrite(uint8_t address, uint16_t data) {
  // set the PHY register address
  _enc28j60_writeReg(MIREGADR, address);

  // write the PHY data
  _enc28j60_writeRegPair(MIWRL, data);

  // wait until the PHY write completes
  while(_enc28j60_readReg(MISTAT) & MISTAT_BUSY) {
    delay_us(15);
  }
}

uint16_t _enc28j60_phyRead(uint8_t address) {
  uint16_t data;

  // Set the right address and start the register read operation
  _enc28j60_writeReg(MIREGADR, address);
  _enc28j60_writeReg(MICMD, MICMD_MIIRD);

  // wait until the PHY read completes
  while(_enc28j60_readReg(MISTAT) & MISTAT_BUSY);

  // quit reading
  _enc28j60_writeReg(MICMD, 0x00);

  // get data value
  data = _enc28j60_readReg(MIRDL);
  data |= ((uint16_t)_enc28j60_readReg(MIRDH)) << 8;
  return data;
}

void _enc28j60_setBank(uint8_t address) {
  // set the bank (if needed)
  if((address & BANK_MASK) != _enc28j60_bank) {
    _enc28j60_writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_BSEL1 | ECON1_BSEL0);
    _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK) >> 5);
    _enc28j60_bank = (address & BANK_MASK);
  }
}

uint8_t _enc28j60_readReg(uint8_t address) {
  // set the bank
  _enc28j60_setBank(address);

  // do the read
  return _enc28j60_readOp(ENC28J60_READ_CTRL_REG, address);
}

void _enc28j60_packetSend(uint8_t* packet1, uint16_t packet1Length, uint8_t* packet2, uint16_t packet2Length) {
#ifdef ENC28J60_DEBUG
  int i;
  debug_write_line("packetSend:");
  debug_write("  packet1 ");
  debug_write_u16(packet1Length, 10);
  debug_write(": ");
  for(i = 0; i < packet1Length; i++) {
    debug_write_u8(packet1[i], 16);
    debug_write(" ");
  }
  debug_write_line("");
  debug_write("  packet2 ");
  debug_write_u16(packet2Length, 10);
  debug_write(": ");
  for(i = 0; i < packet2Length; i++) {
    debug_write_u8(packet2[i], 16);
    debug_write(" ");
  }
  debug_write_line("");
#endif

  // Errata: Transmit Logic reset
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);

  // Set the write pointer to start of transmit buffer area
  _enc28j60_writeReg(EWRPTL, TXSTART_INIT & 0xff);
  _enc28j60_writeReg(EWRPTH, TXSTART_INIT >> 8);

  // Set the TXND pointer to correspond to the packet size given
  _enc28j60_writeReg(ETXNDL, (TXSTART_INIT + packet1Length + packet2Length));
  _enc28j60_writeReg(ETXNDH, (TXSTART_INIT + packet1Length + packet2Length) >> 8);

  // write per-packet control byte
  _enc28j60_writeOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);

  // copy the packet into the transmit buffer
  _enc28j60_writeBuffer(packet1, packet1Length);
  if(packet2Length > 0) {
    _enc28j60_writeBuffer(packet2, packet2Length);
  }

  // send the contents of the transmit buffer onto the network
  _enc28j60_writeOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}

uint8_t _enc28j60_readOp(uint8_t op, uint8_t address) {
  uint8_t  result;

  enc28j60_spi_assert();

  enc28j60_spi_transfer(op | (address & ADDR_MASK)); // issue read command
  result = enc28j60_spi_transfer(0x00);

  // do dummy read if needed (for mac and mii, see datasheet page 29)
  if(address & 0x80) {
    result = enc28j60_spi_transfer(0x00);
  }

  enc28j60_spi_deassert();
  return result;
}

uint16_t _enc28j60_readOp16(uint8_t op, uint8_t address) {
  uint16_t result;
  result = _enc28j60_readOp(op, address);
  result |= ((uint16_t)_enc28j60_readOp(op, address)) << 8;
  return result;
}

void _enc28j60_writeOp(uint8_t op, uint8_t address, uint8_t data) {
  enc28j60_spi_assert();
  enc28j60_spi_transfer(op | (address & ADDR_MASK));
  enc28j60_spi_transfer(data);
  enc28j60_spi_deassert();
}

void _enc28j60_readBuffer(uint8_t* data, uint16_t len) {
  enc28j60_spi_assert();

  enc28j60_spi_transfer(ENC28J60_READ_BUF_MEM);
  while(len--) {
    *data++ = enc28j60_spi_transfer(0x00);
  }

  enc28j60_spi_deassert();
}

void _enc28j60_writeBuffer(uint8_t* data, uint16_t len) {
  enc28j60_spi_assert();

  enc28j60_spi_transfer(ENC28J60_WRITE_BUF_MEM);
  while(len--) {
    enc28j60_spi_transfer(*data++);
  }

  enc28j60_spi_deassert();
}

#endif
