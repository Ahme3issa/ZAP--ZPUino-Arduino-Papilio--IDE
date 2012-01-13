/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifdef __AVR__
#include "pins_arduino.h"
#endif

#include "SPI.h"

SPIClass SPI;

#ifdef __AVR__

void SPIClass::begin() {
  // Set direction register for SCK and MOSI pin.
  // MISO pin automatically overrides to INPUT.
  // When the SS pin is set as OUTPUT, it can be used as
  // a general purpose output port (it doesn't influence
  // SPI operations).

  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SS, OUTPUT);
  
  digitalWrite(SCK, LOW);
  digitalWrite(MOSI, LOW);
  digitalWrite(SS, HIGH);

  // Warning: if the SS pin ever becomes a LOW INPUT then SPI 
  // automatically switches to Slave, so the data direction of 
  // the SS pin MUST be kept as OUTPUT.
  SPCR |= _BV(MSTR);
  SPCR |= _BV(SPE);
}

void SPIClass::end() {
  SPCR &= ~_BV(SPE);
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
  if(bitOrder == LSBFIRST) {
    SPCR |= _BV(DORD);
  } else {
    SPCR &= ~(_BV(DORD));
  }
}

void SPIClass::setDataMode(uint8_t mode)
{
  SPCR = (SPCR & ~SPI_MODE_MASK) | mode;
}

void SPIClass::setClockDivider(uint8_t rate)
{
  SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
  SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}

#endif

#ifdef ZPU


void SPIClass::begin(int mosi, int miso, int sck)
{
	USPICTL=BIT(SPICP1)|BIT(SPIEN)|BIT(SPIBLOCK);


	pinMode(mosi, OUTPUT);
	pinModePPS(mosi,HIGH);
	pinMode(miso, INPUT);
	pinMode(sck, OUTPUT);
	pinModePPS(sck,HIGH);


	outputPinForFunction( mosi, IOPIN_USPI_MOSI );
	outputPinForFunction( sck, IOPIN_USPI_SCK);
	inputPinForFunction( miso, IOPIN_USPI_MISO );

}

void SPIClass::end() {
	USPICTL &= ~(_BV(SPIEN)|_BV(SPIBLOCK));
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
	/* No support */
}

void SPIClass::setDataMode(uint8_t mode)
{
	USPICTL = (USPICTL & ~SPI_MODE_MASK) | mode;
}

void SPIClass::setClockDivider(uint8_t rate)
{
	USPICTL = (USPICTL & ~SPI_CLOCK_MASK) | rate;
}

#endif
