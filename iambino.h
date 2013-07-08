// Iambino - Iambic Keyer for Arduino
// Copyright (C) 2013 David Turnbull AE9RB
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


// Feel free to change these for more or less adjustability.
#define CFG_VOLUME_MAX 20
#define CFG_BACKLIGHT_MAX 10
#define CFG_SPEED_MIN 5.0
#define CFG_SPEED_MAX 40.0
#define CFG_LAG_MAX 25
#define CFG_WEIGHT_DIST 0.25

// Keying modes
#define CFG_MODE_IAMBIC 0
#define CFG_MODE_ULTIMATIC 1
#define CFG_MODE_BUG 2
#define CFG_MODE_STRAIGHT 3

// Keying memory
#define CFG_MEMORY_NONE 0
#define CFG_MEMORY_DAH 1
#define CFG_MEMORY_DIT 2
#define CFG_MEMORY_BOTH 3

// Keying spacing
#define CFG_SPACING_NONE 0
#define CFG_SPACING_EL 1
#define CFG_SPACING_CHAR 2
#define CFG_SPACING_WORD 3

// Paddle reversal
#define CFG_PADDLE_NORMAL 0
#define CFG_PADDLE_REVERSE 1

// Morse code timings.
#define DIT 1
#define DAH 3

// Spaces for clearing LCD. Send twice to clear a whole line.
#define LCD_CLEAR_8  "        "

// Button press events. First press will have BUTTON_REPEAT clear.
// Holding a button will generate events with BUTTON_REPEAT set.
#define BUTTON_NONE     0
#define BUTTON_SELECT   _BV(0)
#define BUTTON_LEFT     _BV(1)
#define BUTTON_DOWN     _BV(2)
#define BUTTON_UP       _BV(3)
#define BUTTON_RIGHT    _BV(4)
#define BUTTON_RELEASE  _BV(6)
#define BUTTON_REPEAT   _BV(7)

// Buttons pin. Divider array on analog pin.
#define BUTTON_APIN A2

// Buttons pin. Divider array on analog pin.
#define POT_APIN A3

// Iambic key pins. 0=dot, 1=dash
#define KEY_0 3
#define KEY_1 2

// Transmit pin
#define TX_PIN 14

// LCD pins
#define LCD_B4 9
#define LCD_B5 8
#define LCD_B6 7
#define LCD_B7 6
#define LCD_RS 15
#define LCD_E  4
#define LCD_BACKLIGHT 5

// DAC pins for direct hardware access.
// Arduino digitalWrite() calls will lookup pins from a
// table in program space. We avoid this lookup and the
// stack calls by accessing hardware directly.
// PIN11
#define DAC_SDI_DDR  DDRB
#define DAC_SDI_PORT PORTB
#define DAC_SDI_BIT  (3)
// PIN12
#define DAC_CS_DDR  DDRB
#define DAC_CS_PORT PORTB
#define DAC_CS_BIT  (4)
// PIN13
#define DAC_SCK_DDR  DDRB
#define DAC_SCK_PORT PORTB
#define DAC_SCK_BIT  (5)
