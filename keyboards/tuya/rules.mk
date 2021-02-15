# MCU name
MCU = STM32F042

# Bootloader selection
#   Teensy       halfkay
#   Pro Micro    caterina
#   Atmel DFU    atmel-dfu
#   LUFA DFU     lufa-dfu
#   QMK DFU      qmk-dfu
#   ATmega32A    bootloadHID
#   ATmega328P   USBasp

# If you don't know the bootloader type, then you can specify the
# Boot Section Size in *bytes* by uncommenting out the OPT_DEFS line
# Otherwise, delete this section
#   Teensy halfKay      512
#   Teensy++ halfKay    1024
#   Atmel DFU loader    4096
#   LUFA bootloader     4096
#   USBaspLoader        2048
# OPT_DEFS += -DBOOTLOADER_SIZE=4096

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no       # Virtual DIP switch configuration
MOUSEKEY_ENABLE = no # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no			# Console for debug
COMMAND_ENABLE = no			# Commands for debug and configuration
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = yes			# USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
MIDI_ENABLE = no            # MIDI support
BLUETOOTH_ENABLE = no       # Enable Bluetooth with the Adafruit EZ-Key HID

# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE

OLED_DRIVER_ENABLE = yes

SRC += system.c
SRC += protocol.c
SRC += mcu_api.c
SRC += serial_tuya.c
