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
#include "i2c_master.h"
#include "serial_tuya.h"
#include <string.h>
#include <hal.h>


static const SerialConfig uart_config = {
    115200,
    0,
    USART_CR2_STOP1_BITS,
    0
};

static uart_status_t chibios_to_qmk(const msg_t* status) {
    switch (*status) {
        case 0:
            return 0;
        //case I2C_TIMEOUT:
        //    return -2;
        // I2C_BUS_ERROR, I2C_ARBITRATION_LOST, I2C_ACK_FAILURE, I2C_OVERRUN, I2C_PEC_ERROR, I2C_SMB_ALERT
        default:
            return -1;
    }
}

__attribute__((weak)) void uart_init(void) {
    // Try releasing special pins for a short time
    palSetPadMode(GPIOA, 9, PAL_MODE_INPUT);
    palSetPadMode(GPIOA, 10, PAL_MODE_INPUT);

    chThdSleepMilliseconds(10);

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(1));
    palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(1));
}

uart_status_t uart_start(void) {
    sdStart(&UART_DRIVER, &uart_config);
    return 0;
}

uart_status_t uart_tx(const uint8_t* data, uint16_t length) {
    msg_t status = 0;

    //sdStart(&UART_DRIVER, &uart_config);
    #if 0
    if(length != sdWriteTimeout(&UART_DRIVER, data, length, TIME_IMMEDIATE))
    {
        status = -1;
    }
    #else
    sdWrite(&UART_DRIVER, data, length);
    #endif
    return chibios_to_qmk(&status);
}

uart_status_t uart_rx(uint8_t* data, uint16_t length) {
    msg_t status = 0;

    //sdStart(&UART_DRIVER, &uart_config);
    if(length != sdReadTimeout(&UART_DRIVER, data, length, TIME_IMMEDIATE))
    {
        status = -1;
    }
    return chibios_to_qmk(&status);
}

void uart_stop(void) { sdStop(&UART_DRIVER); }
