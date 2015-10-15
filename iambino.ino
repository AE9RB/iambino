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

#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "iambino.h"

void setup() {
  pinMode( TX_PIN, OUTPUT );
  pinMode( LCD_BACKLIGHT, OUTPUT );
  digitalWrite(KEY_0, HIGH);
  digitalWrite(KEY_1, HIGH);

  Serial.begin(9600);
  lcd_setup();
  cfg_setup();
  dac_setup();
}

void loop() {
  static uint8_t state=0;
  static float prev_speed;
  long mark;
  uint8_t button, mcode;
  
  button_pot();
  if (prev_speed != cfg_get_speed()) {
    prev_speed = cfg_get_speed();
    switch(state) {
    default:
      lcd_show_main();
      break;
    case 1: // settings
      cfg_loop(BUTTON_SELECT);
      break;
    case 2: // play
      lcd_show_play();
      break;
    case 4: // rec
      lcd_show_record();
      break;
    }
  }
  
  mark = micros();
  switch(state) {
  default: // normal & settings
    mcode = key_loop(mark);
    break;
  case 2: // message play
  case 3:
    mcode = message_play(mark);
    if (mcode == 1) {
      lcd_show_main();
      mcode = 0;
      state = 0;
    }
    break;
  case 4: // message record
    mcode = message_record(key_loop(mark));
    break;
  }
  dac_loop(mark);
  tx_loop(mark);
  if (mcode == 1) lcd_write(1);
  else if (mcode) lcd_write(morse_char_for(mcode));
  if (state != 1) lcd_loop();
  
  button = button_read(mark);
  switch(state) {
  case 0: // normal
    if (button & (BUTTON_LEFT|BUTTON_DOWN|BUTTON_UP|BUTTON_RIGHT)) {
      if (button & BUTTON_RELEASE) {
        message_load(button);
        lcd_show_play();
        state = 2;
      }
      if (button & BUTTON_REPEAT) {
        message_load(button);
        lcd_clear();
        lcd_show_record();
        tx_disable();
        state = 4;
      }
    }
    if (button == BUTTON_SELECT) {
      lcd_show_settings();
      cfg_loop(button);
      state = 1;
    }
    break;
  case 1: // settings
    if (button && !(button & BUTTON_RELEASE)) cfg_loop(button);
    if (button == BUTTON_SELECT) {
      lcd_show_main();
      cfg_save();
      state = 0;
    }
    break;
  case 2: // message play
    if (button & BUTTON_RELEASE || key_read()) {
      message_stop_play();
      lcd_show_main();
      state = 3;
    }
    break;
  case 3: // stopping play
    break;
  case 4: // message record
    if (button & (BUTTON_LEFT|BUTTON_DOWN|BUTTON_UP|BUTTON_RIGHT) &&
        !(button & (BUTTON_REPEAT|BUTTON_RELEASE))) {
      message_save();
      state = 5;
    }
    else if (button != (BUTTON_SELECT|BUTTON_RELEASE)) break;
    lcd_show_main();
    //nobreak;
  case 5: // stopping record
    if (button & BUTTON_RELEASE) {
      tx_enable();
      state = 0;
    }
    break;
  }
  
}

long tx_timer;
bool tx_flag = true;

void tx_send(long mark) {
  if (tx_flag) digitalWrite(TX_PIN, HIGH);
  tx_timer = mark;
}

void tx_loop(long mark) {
  if(tx_timer - mark < 0) digitalWrite(TX_PIN, LOW);
}

bool tx_enabled() {
  return tx_flag;
}

void tx_enable() {
  tx_flag = true;
}

void tx_disable() {
  tx_flag = false;
}


// Transfer data eeprom-to-memory or memory-to-eeprom.
// Skips writes where the data has not changed.
void eeprom_xfer(void* mem, int eeprom, size_t length, bool write) {
  uint8_t i, j;
  while (length--) {
    i = EEPROM.read(eeprom);
    if (write) {
      j = *(uint8_t*)mem;
      if (i!=j) EEPROM.write(eeprom, j);
    } else {
      *(uint8_t*)mem = i;
    }
    mem = (uint8_t*)mem + 1;
    eeprom++;
  }
}

// A basic CRC for validating the cfg struct
// Polynomial: x^8 + x^5 + x^4 + 1 (0x8C)
uint8_t eeprom_crc8(const void *data, uint8_t size)
{
  uint8_t bit;
  uint8_t crc = 0x00;
  while(size--) {
    crc ^= *(uint8_t*)data;
    data = ((uint8_t*)data) + 1;
    for (bit=8; bit; bit--) {
      if (crc & 0x01) crc = (crc >> 1) ^ 0x8C;
      else crc >>= 1;
    }
  }
  return crc;
}


typedef void(*cfg_function)(uint8_t);
cfg_function cfg_functions[] = {
  cfg_speed,
  cfg_speed_min,
  cfg_speed_max,
  cfg_message,
  cfg_mode,
  cfg_memory,
  cfg_spacing,
  cfg_weight,
  cfg_paddle,
  cfg_lag,
  cfg_tone,
  cfg_volume,
  cfg_sidetone,
  cfg_backlight
};

struct cfg {
  uint8_t speed_min;
  uint8_t speed_max;
  uint8_t message;
  int8_t  mode;
  int8_t  memory;
  int8_t  spacing;
  float   weight;
  uint8_t paddle;
  uint8_t lag;
  float   tone;
  uint8_t volume;
  uint8_t sidetone;
  uint8_t backlight;
} cfg;

void cfg_reset() {
  int8_t i = sizeof(cfg_functions)/sizeof(cfg_function);
  while(i--) cfg_functions[i](BUTTON_NONE);  
}

void cfg_save() {
  uint8_t crc = eeprom_crc8(&cfg, sizeof(cfg));
  eeprom_xfer(&cfg, 0, sizeof(cfg), true);
  if (crc != EEPROM.read(sizeof(cfg))) EEPROM.write(sizeof(cfg), crc);
}

void cfg_loop(uint8_t button) {
  static int8_t setting = 0;
  
  if (button & (BUTTON_LEFT | BUTTON_RIGHT | BUTTON_SELECT)) button_fast(false);
  if (button & BUTTON_LEFT) setting--;
  if (button & BUTTON_RIGHT) setting++;
  if (setting < 0) setting = sizeof(cfg_functions)/sizeof(cfg_function)-1;
  if (setting >= (int8_t)(sizeof(cfg_functions)/sizeof(cfg_function))) setting = 0;
  cfg_functions[setting](button);
}

void cfg_setup() {
  long mark;
  uint8_t button;
  int8_t selection = 0;
  
  eeprom_xfer(&cfg, 0, sizeof(cfg), false);
  if (eeprom_crc8(&cfg, sizeof(cfg)) != EEPROM.read(sizeof(cfg))) {
    cfg_reset();
  }

  mark = micros();
  button_read(mark - 100000);
  button = button_read(mark);
  
  if (button == BUTTON_SELECT) {
    lcd_set_backlight(CFG_BACKLIGHT_MAX);
    lcd.setCursor(0, 0);
    lcd.print(F("\001 ERASE EEPROM \002"));
    for(;;) {
      lcd.setCursor(0, 1);
      switch(selection) {
        case 0:
          lcd.print(F("\001    CANCEL    \002"));
          break;
        case 1:
          lcd.print(F("\001   SETTINGS   \002"));
          break;
        case 2:
          lcd.print(F("\001  EVERYTHING  \002"));
          break;
      }
      mark = micros();
      button = button_read(mark);
      if (button == BUTTON_LEFT || button == BUTTON_DOWN) selection--;
      if (button == BUTTON_RIGHT || button == BUTTON_UP) selection++;
      if (selection > 2) selection = 0;
      if (selection < 0) selection = 2;

      if (button == BUTTON_SELECT) {
        lcd_set_backlight(0);
        if (selection==2) {
          for (int i=0; i<1024; i++) EEPROM.write(i, 0xFF);
        }
        if (selection) {
          cfg_reset();
          cfg_save();
        }
        break;
      }
    }
  }
  
  cfg_set_speed(0);
  cfg_set_tone(cfg.tone);
  cfg_set_volume(cfg.volume);
  cfg_set_backlight(cfg.backlight);  
}
