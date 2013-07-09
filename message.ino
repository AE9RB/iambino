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

uint8_t message_length, message_data[111];
uint8_t message_index, message_pos, message_char, message_char_cnt;

uint8_t cfg_message_bank;

uint8_t cfg_get_message() {
  return cfg_message_bank;
}

void cfg_set_message(uint8_t bank) {
  cfg_message_bank = bank;
}

void cfg_message(uint8_t button) {
  if (button & (BUTTON_UP|BUTTON_DOWN)) {
    cfg_message_bank = (cfg_message_bank + 1) & 1;
  }
  
  lcd.setCursor( 0, 1 );
  lcd.print("MESSAGE BANK: ");
  lcd.print(cfg_message_bank + 1);
  lcd.print(LCD_CLEAR_8);
}

uint8_t message_record(uint8_t mcode) {
  uint8_t i;
  if (!mcode || mcode == 0xff) return 0;
  if (message_pos >= sizeof(message_data)) return 0;
  if (mcode == 0x01) {
    // delete
    if (message_pos > 0 && message_data[message_pos-1] == 0x80) message_pos--;
    while(message_pos > 0) {
      if (message_data[message_pos-1] == 0x80) break;
      message_pos--;
    }
    lcd_clear();
    if (message_pos < 16) i = 0;
    else i = message_pos - 16;
    for (;i<message_pos;i++) {
      lcd_write(morse_char_for(message_data[i]));
    }
    return 0;
  }
  if (mcode != 0x80 || (message_pos > 0 && message_data[message_pos-1] != 0x80)) {
    message_data[message_pos] = mcode;
    message_pos++;
    return mcode;
  }
  return 0;
}

void message_eeprom_xfer(bool write) {
  int i, j = message_index * (sizeof(message_length) + sizeof(message_data));
  if (write) EEPROM.write(j, message_length);
  else message_length = EEPROM.read(j);
  for(i=0; i < (int)sizeof(message_data); i++) {
    j++;
    if (write) EEPROM.write(j, message_data[i]);
    else message_data[i] = EEPROM.read(j);
  }
}

void message_save() {
  if (message_pos > 0 && message_data[message_pos-1] == 0x80) {
    message_pos--;
  }
  message_length = message_pos;
  message_eeprom_xfer(true);
}

void message_stop_play() {
  message_pos = message_length;
}

uint8_t message_play(long mark) {
  static uint8_t state = 0;
  static long wait;
  long i;
  uint8_t ret = 0;
  
  if (!message_char_cnt) {
    if (message_pos >= message_length) return 1;
    if (!message_pos) wait = mark;
    ret = message_char = message_data[message_pos];
    message_pos++;
    if (!message_char) {
       message_char_cnt = 1;
    } else {
      message_char_cnt = 8;
      while (!(message_char & 0x01)) {
        message_char >>= 1;
        message_char_cnt -= 1;
      }
    }
  }
  
  if (wait - mark < 0) switch(state) {
  case 0:
    state = 1;
    if (message_char_cnt == 1) {
      wait = mark + 2 * DIT * cfg_get_speed_micros();
      break;
    }
    //nobreak;
  case 1:
    message_char >>= 1;
    message_char_cnt -= 1;
    if (message_char_cnt) {
      if (message_char & 0x01) i = mark + DAH * cfg_get_speed_micros();
      else i = mark + DIT * cfg_get_speed_micros();
      i += DIT * cfg_get_speed_micros() * (cfg_get_weight() * 2 - 1);
      dac_play(i);
      tx_send(i + (long)cfg_get_lag() * 1000);
      wait = i + DIT * cfg_get_speed_micros() * (2.0 - cfg_get_weight() * 2);
    } else {
      wait = mark + 2 * DIT * cfg_get_speed_micros();
      if (message_pos >= message_length) ret = 0x80;
    }
    if (!message_char_cnt) state = 0;
    break;
  }
  
  return ret;
}

void message_load(uint8_t button) {
  switch (button & (BUTTON_LEFT | BUTTON_DOWN | BUTTON_UP | BUTTON_RIGHT)) {
    case BUTTON_LEFT: message_index = 1; break;
    case BUTTON_DOWN: message_index = 2; break;
    case BUTTON_UP: message_index = 3; break;
    case BUTTON_RIGHT: message_index = 4; break;
  }
  message_index += 4 * cfg_message_bank;
  message_eeprom_xfer(false);
  if (message_length > sizeof(message_data)) message_length = 0;
  message_pos = 0;
  message_char_cnt = 0;  
}

uint8_t message_get_index() {
  return message_index;
}


