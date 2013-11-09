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
#define CFG_SPEED_MAX 50.0
#define CFG_LAG_MAX 25
#define CFG_WEIGHT_DIST 0.25

// Morse code timings.
#define DIT 1
#define DAH 3

// Spaces for clearing LCD.
#define LCD_CLEAR_8  "        "

// Button press events.
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

// Speed potentiometer.
#define POT_APIN A3

// Iambic key pins. 0=dot, 1=dash
#define KEY_0 3
#define KEY_1 2

// Transmit pin
#define TX_PIN A0

// LCD pins
#define LCD_B4 9
#define LCD_B5 8
#define LCD_B6 7
#define LCD_B7 6
#define LCD_RS A1
#define LCD_E  4
#define LCD_BACKLIGHT 5

// DAC pins for direct hardware access.
#if defined(__AVR_ATmega32U4__)
// PIN11
#define DAC_SDI_DDR  DDRB
#define DAC_SDI_PORT PORTB
#define DAC_SDI_BIT  (7)
// PIN12
#define DAC_CS_DDR  DDRD
#define DAC_CS_PORT PORTD
#define DAC_CS_BIT  (6)
// PIN13
#define DAC_SCK_DDR  DDRC
#define DAC_SCK_PORT PORTC
#define DAC_SCK_BIT  (7)
#else /* ATmega328 */
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
#endif


// Share lcd object across entire app
extern LiquidCrystal lcd;

// Update the progmem macros so they don't spam warnings
#if (((__GNUC__ * 1000) + __GNUC_MINOR__) * 1000 + __GNUC_PATCHLEVEL__) < 4006002 
# undef PROGMEM 
# define PROGMEM __attribute__(( section(".progmem.data") )) 
# undef PSTR 
# define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s);&__c[0];})) 
#endif 
