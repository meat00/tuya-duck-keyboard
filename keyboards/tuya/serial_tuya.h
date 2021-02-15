/* Copyright 2018 Jack Humbert
 * Copyright 2018 Yiancar
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* This library is only valid for STM32 processors.
 * This library follows the convention of the AVR i2c_master library.
 * As a result addresses are expected to be already shifted (addr << 1).
 * I2CD1 is the default driver which corresponds to pins B6 and B7. This
 * can be changed.
 * Please ensure that HAL_USE_I2C is TRUE in the halconf.h file and that
 * STM32_I2C_USE_I2C1 is TRUE in the mcuconf.h file. Pins B6 and B7 are used
 * but using any other I2C pins should be trivial.
 */
#include "quantum.h"
#ifndef __SERIAL_TUYA_H__
#define __SERIAL_TUYA_H__

#ifndef UART_DRIVER
#    define UART_DRIVER SD1
#endif


typedef uint16_t uart_status_t;

void uart_init(void);
uart_status_t uart_start(void);
uart_status_t uart_tx(const uint8_t* data, uint16_t length);
uart_status_t uart_rx(uint8_t* data, uint16_t length);

void uart_stop(void);

#endif  /* __SERIAL_TUYA_H__ */
