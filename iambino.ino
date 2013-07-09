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

#include "iambino.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

void setup() {
  pinMode( TX_PIN, OUTPUT );
  pinMode( LCD_BACKLIGHT, OUTPUT );
  digitalWrite(KEY_0, HIGH);
  digitalWrite(KEY_1, HIGH);

  Serial.begin(9600);
  settings_reset();
  dac_setup();
  lcd_setup();  
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
    case 0: // normal
      lcd_show_main();
      break;
    case 1: // settings
      settings_loop(BUTTON_NONE);
      break;
    case 2: // play
      lcd_show_play();
      break;
    default: // record
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
    mcode = message_play(mark);
    if (mcode == 1) {
      lcd_show_main();
      mcode = 0;
      state = 0;
    }
    break;
  case 3: // message record
    mcode = message_record(key_loop(mark));
    break;
  }
  dac_loop(mark);
  tx_loop(mark);
  if (mcode == 1) lcd_write(1);
  else if (mcode) lcd_write(morse_char_for(mcode));
  
  button = button_read(mark);
  switch(state) {
  case 0: // normal
    lcd_loop();
    if (button & (BUTTON_LEFT | BUTTON_DOWN | BUTTON_UP | BUTTON_RIGHT)) {
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
        state = 3;
      }
    }
    if (button == BUTTON_SELECT) {
      settings_loop(button);
      lcd_show_settings();
      state = 1;
    }
    break;
  case 1: // settings
    if (button && !(button & BUTTON_RELEASE)) settings_loop(button);
    if (button == BUTTON_SELECT) {
      lcd_show_main();
      state = 0;
    }
    break;
  case 2: // message play
    lcd_loop();
    if (button & BUTTON_RELEASE || key_read()) {
      message_stop_play();
    }
    break;
  case 3: // message record
    lcd_loop();
    if (button & BUTTON_SELECT) {
      if (button & BUTTON_RELEASE) {
        // cancel record
        lcd_show_main();
        tx_enable();
        state = 0;
      }
    }
    else if (button && !(button & (BUTTON_REPEAT|BUTTON_RELEASE))) {
      message_save();
      state = 4;
    }
    break;
  case 4: // stop record
    if (button & BUTTON_RELEASE) {
      lcd_show_main();
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


void settings_reset() {
  cfg_set_speed(0);
  cfg_set_speed_min(CFG_SPEED_MIN);
  cfg_set_speed_max(CFG_SPEED_MAX);
  cfg_set_tone(1318.510);
  cfg_set_volume(5);
  cfg_set_mode(CFG_MODE_IAMBIC);
  cfg_set_memory(CFG_MEMORY_BOTH);
  cfg_set_backlight(6);
  cfg_set_lag(0);
  cfg_set_weight(0.50);
  cfg_set_spacing(CFG_SPACING_EL);
  cfg_set_paddle(CFG_PADDLE_NORMAL);
  cfg_set_sidetone(1);
  cfg_set_message(0);
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

void settings_loop(uint8_t button) {
  static int8_t setting = 0;
  
  if (button & (BUTTON_LEFT | BUTTON_RIGHT | BUTTON_SELECT)) button_fast(false);
  if (button & BUTTON_LEFT) setting--;
  if (button & BUTTON_RIGHT) setting++;
  if (setting < 0) setting = sizeof(cfg_functions)/sizeof(cfg_function)-1;
  if (setting >= (int8_t)(sizeof(cfg_functions)/sizeof(cfg_function))) setting = 0;
  cfg_functions[setting](button);
}