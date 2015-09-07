/*
  serialUtils.h - Definitions and utility functions for the Serial feature.
  Copyright (c) 2015 Jeff Hoefs.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.

  Last update September 6th, 2015
*/

#ifndef SERIAL_UTILS_H
#define SERIAL_UTILS_H

// Serial port Ids
#define HW_SERIAL0                  0x00
#define HW_SERIAL1                  0x01
#define HW_SERIAL2                  0x02
#define HW_SERIAL3                  0x03
// extensible up to 0x07

#define SW_SERIAL0                  0x08
#define SW_SERIAL1                  0x09
#define SW_SERIAL2                  0x0A
#define SW_SERIAL3                  0x0B
// extensible up to 0x0F

#define SERIAL_PORT_ID_MASK         0x0F
#define MAX_SERIAL_PORTS            8

// map configuration query response resolution value to serial pin type
#define RES_RX1                     0x02
#define RES_TX1                     0x03
#define RES_RX2                     0x04
#define RES_TX2                     0x05
#define RES_RX3                     0x06
#define RES_TX3                     0x07

// Serial command bytes
#define SERIAL_CONFIG               0x10
#define SERIAL_WRITE                0x20
#define SERIAL_READ                 0x30
#define SERIAL_REPLY                0x40
#define SERIAL_CLOSE                0x50
#define SERIAL_FLUSH                0x60
#define SERIAL_LISTEN               0x70

// Serial read modes
#define SERIAL_READ_CONTINUOUSLY    0x00
#define SERIAL_STOP_READING         0x01
#define SERIAL_MODE_MASK            0xF0

// Serial options bit masks
#define SERIAL_DATA_BITS_MASK       0x01E0
#define SERIAL_PARITY_MASK          0x001C
#define SERIAL_STOP_BITS_MASK       0x0003

struct serial_pins {
  uint8_t rx;
  uint8_t tx;
};

/*
 * Get the serial serial pin type (RX1, TX1, RX2, TX2, etc) for the specified pin.
 */
inline uint8_t getSerialPinType(uint8_t pin) {
#if defined(PIN_SERIAL_RX)
  // TODO when use of HW_SERIAL0 is enabled
#endif
#if defined(PIN_SERIAL1_RX)
  if (pin == PIN_SERIAL1_RX) return RES_RX1;
  if (pin == PIN_SERIAL1_TX) return RES_TX1;
#endif
#if defined(PIN_SERIAL2_RX)
  if (pin == PIN_SERIAL2_RX) return RES_RX2;
  if (pin == PIN_SERIAL2_TX) return RES_TX2;
#endif
#if defined(PIN_SERIAL3_RX)
  if (pin == PIN_SERIAL3_RX) return RES_RX3;
  if (pin == PIN_SERIAL3_TX) return RES_TX3;
#endif
  return 0;
}

/*
 * Get the RX and TX pins numbers for the specified HW serial port.
 */
inline serial_pins getSerialPinNumbers(uint8_t portId) {
  serial_pins pins;
  switch (portId) {
#if defined(PIN_SERIAL_RX)
    // case HW_SERIAL0:
    //   // TODO when use of HW_SERIAL0 is enabled
    //   break;
#endif
#if defined(PIN_SERIAL1_RX)
    case HW_SERIAL1:
      pins.rx = PIN_SERIAL1_RX;
      pins.tx = PIN_SERIAL1_TX;
    break;
#endif
#if defined(PIN_SERIAL2_RX)
    case HW_SERIAL2:
      pins.rx = PIN_SERIAL2_RX;
      pins.tx = PIN_SERIAL2_TX;
    break;
#endif
#if defined(PIN_SERIAL3_RX)
    case HW_SERIAL3:
      pins.rx = PIN_SERIAL3_RX;
      pins.tx = PIN_SERIAL3_TX;
    break;
#endif
    default:
      pins.rx = 0;
      pins.tx = 0;
  }
  return pins;
}

/*
 * Get the Arduino serial config value.
 */
inline uint16_t getSerialConfigValue(uint16_t serialOptions) {
  uint8_t dataBits, parity, stopBits;
  dataBits = (serialOptions & SERIAL_DATA_BITS_MASK) >> 5;
  parity = (serialOptions & SERIAL_PARITY_MASK) >> 2;
  stopBits = (serialOptions & SERIAL_STOP_BITS_MASK); // 0 = 1, 1 = 1.5, 2 = 2

  // Arduino does not support 1.5 stop bits so only configurations with 1 and 2 stop bits
  // are returned here

  switch (parity) {
    case 0:
      if (stopBits == 0) {
        switch (dataBits) {
          case 5:
            return SERIAL_5N1;
          case 6:
            return SERIAL_6N1;
          case 7:
            return SERIAL_7N1;
          case 8:
            return SERIAL_8N1;
        }
      } else if (stopBits == 2) {
        switch (dataBits) {
          case 5:
            return SERIAL_5N2;
          case 6:
            return SERIAL_6N2;
          case 7:
            return SERIAL_7N2;
          case 8:
            return SERIAL_8N2;
        }
      }
      break;
    case 1:
      if (stopBits == 0) {
        switch (dataBits) {
          case 5:
            return SERIAL_5E1;
          case 6:
            return SERIAL_6E1;
          case 7:
            return SERIAL_7E1;
          case 8:
            return SERIAL_8E1;
        }
      } else if (stopBits == 2) {
        switch (dataBits) {
          case 5:
            return SERIAL_5E2;
          case 6:
            return SERIAL_6E2;
          case 7:
            return SERIAL_7E2;
          case 8:
            return SERIAL_8E2;
        }
      }
      break;
    case 2:
      if (stopBits == 0) {
        switch (dataBits) {
          case 5:
            return SERIAL_5O1;
          case 6:
            return SERIAL_6O1;
          case 7:
            return SERIAL_7O1;
          case 8:
            return SERIAL_8O1;
        }
      } else if (stopBits == 2) {
        switch (dataBits) {
          case 5:
            return SERIAL_5O2;
          case 6:
            return SERIAL_6O2;
          case 7:
            return SERIAL_7O2;
          case 8:
            return SERIAL_8O2;
        }
      }
      break;
#if defined(ARDUINO_ARCH_SAM)
    case 3:
      // Only available for SAM core Arduino boards
      if (stopBits == 0) {
        switch (dataBits) {
          case 5:
            return SERIAL_5M1;
          case 6:
            return SERIAL_6M1;
          case 7:
            return SERIAL_7M1;
          case 8:
            return SERIAL_8M1;
        }
      } else if (stopBits == 2) {
        switch (dataBits) {
          case 5:
            return SERIAL_5M2;
          case 6:
            return SERIAL_6M2;
          case 7:
            return SERIAL_7M2;
          case 8:
            return SERIAL_8M2;
        }
      }
      break;
    case 4:
      // Only available for SAM core Arduino boards
      if (stopBits == 0) {
        switch (dataBits) {
          case 5:
            return SERIAL_5S1;
          case 6:
            return SERIAL_6S1;
          case 7:
            return SERIAL_7S1;
          case 8:
            return SERIAL_8S1;
        }
      } else if (stopBits == 2) {
        switch (dataBits) {
          case 5:
            return SERIAL_5S2;
          case 6:
            return SERIAL_6S2;
          case 7:
            return SERIAL_7S2;
          case 8:
            return SERIAL_8S2;
        }
      }
      break;
#endif
  }

  // default
  return SERIAL_8N1;
}

#endif /* SERIAL_UTILS_H */
