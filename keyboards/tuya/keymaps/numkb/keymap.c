/* Copyright 2020 datou
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
#ifdef __INTELLISENSE__
    #include "../../config.h"
    #include "../../tuya.h"
    #define QMK_KEYBOARD_H "tuya.h"
    #define OLED_DRIVER_ENABLE
#else
    #include QMK_KEYBOARD_H
#endif
#include "protocol.h"
#include "serial_tuya.h"
#include "mcu_api.h"
#include "i2c_master.h"

#define SHT_I2CADDR     (0x44)

#ifdef OLED_DRIVER_ENABLE
void set_keylog(uint16_t keycode, keyrecord_t *record);
#endif

extern void wifi_uart_service(void);
extern void wifi_protocol_init(void);
extern void uart_receive_input(unsigned char value);

uint8_t shot_init[2] = { 0x24, 0x16 };
uint8_t rd_data[6] = {0};
int temper_pre = 0;
int humidity_pre = 0;

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
};

enum custom_keycodes {
    T_MACRO = SAFE_RANGE,//定义一个按键宏
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
        KC_NLCK, KC_PSLS, KC_PAST, KC_PMNS,
        KC_7,   KC_8,   KC_9,   KC_PPLS,
        KC_4,   KC_5,   KC_6,
        KC_1,   KC_2,   KC_3,   KC_ENT,
        KC_0,            KC_PDOT
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef OLED_DRIVER_ENABLE
    if (record->event.pressed) {
        set_keylog(keycode, record);
    }
#endif
    return true;
}

void matrix_init_user(void) {
    i2c_transmit((SHT_I2CADDR << 1), shot_init, 2, I2C_TIMEOUT);

    uart_init();
    sdInit();
    uart_start();
    wifi_protocol_init();
}

void matrix_scan_user(void) {
    static int hst_cnt = 0;
    uint8_t rxval;

    if(0 == uart_rx(&rxval, 1))
    {
        uart_receive_input(rxval);
    }
    wifi_uart_service();

    if(true == duck_flag)
    {
        duck_flag = false;
        switch(duck_val)
        {
            case '1':
                SEND_STRING("hello world\n");
                break;
            default:
                break;
        }
    }

    if(++hst_cnt > 2000)
    {
        hst_cnt = 0;
        i2c_receive((SHT_I2CADDR << 1), rd_data, 6, I2C_TIMEOUT);
        i2c_transmit((SHT_I2CADDR << 1), shot_init, 2, I2C_TIMEOUT);
    }
}

bool temper2str(char *strbuf, uint8_t *buf)
{
    int temper, humidity;
    uint16_t rdval;

    rdval = ((uint16_t)buf[0] << 8) | buf[1];
    temper = (int)((float)1750*(float)rdval/(float)65535 - 450);
    rdval = ((uint16_t)buf[3] << 8) | buf[4];
    humidity = (int)((float)100*(float)rdval/(float)65535);

    if(temper < -255 || temper > 1024 || humidity <=0 || humidity > 90)
    {
        return false;
    }

    snprintf(strbuf, 20, "T:%d H:%d", temper, humidity);

    if (temper_pre != temper)
    {
        mcu_dp_value_update(DPID_TEMP_CURRENT, temper); //VALUE型数据上报;
        temper_pre = temper;
    }
    if(humidity_pre != humidity)
    {
        mcu_dp_value_update(DPID_HUMIDITY_VALUE, humidity); //VALUE型数据上报;
        humidity_pre = humidity;
    }

    return true;
}

#ifdef OLED_DRIVER_ENABLE

char keylog_str[24] = {0};
char keylogs_str[21] = {0};
int keylogs_str_idx = 0;

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ';', '\'', ' ', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);

  // update keylogs
  if (keylogs_str_idx == sizeof(keylogs_str) - 1) {
    keylogs_str_idx = 0;
    for (int i = 0; i < sizeof(keylogs_str) - 1; i++) {
      keylogs_str[i] = ' ';
    }
  }

  keylogs_str[keylogs_str_idx] = name;
  keylogs_str_idx++;
}

const char *read_keylog(void) {
  return keylog_str;
}

const char *read_keylogs(void) {
  return keylogs_str;
}

void oled_task_user(void) {
    char temper_buf[20];

    // Host Keyboard LED Status
    oled_write_P(IS_HOST_LED_ON(USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
    oled_write_P(IS_HOST_LED_ON(USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(IS_HOST_LED_ON(USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK\n") : PSTR("       "), false);

    oled_write_ln(read_keylog(), false);
    oled_write_ln(read_keylogs(), false);

    if (true == temper2str(temper_buf, rd_data))
    {
        oled_write_ln(temper_buf, false);
    }
}
#endif
