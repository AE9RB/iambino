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

LiquidCrystal lcd( LCD_RS, LCD_E, LCD_B4, LCD_B5, LCD_B6, LCD_B7 );

const char PROGMEM lcd_box[] PROGMEM = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};

const char PROGMEM lcd_left_arrow[] PROGMEM = {
  0b00010,
  0b00110,
  0b01110,
  0b11110,
  0b01110,
  0b00110,
  0b00010,
};

const char PROGMEM lcd_right_arrow[] PROGMEM = {
  0b01000,
  0b01100,
  0b01110,
  0b01111,
  0b01110,
  0b01100,
  0b01000,
};

void lcd_create_char(uint8_t location, const char PROGMEM charmap[]) {
  uint8_t buffer[8];
  buffer[7] = 0;
  memcpy_P(buffer, charmap, 7);
  lcd.createChar(location, buffer);
}

void lcd_set_backlight(uint8_t level) {
  float f = 1 - log10(((float)CFG_BACKLIGHT_MAX / 9 + CFG_BACKLIGHT_MAX - level) * 9 / CFG_BACKLIGHT_MAX);
  analogWrite(LCD_BACKLIGHT, 255 * f);
}

uint8_t lcd_buffer[16], lcd_pos = 0, lcd_update_pos = 16;

void lcd_setup() {
  lcd.begin( 16, 2 );
  lcd_clear();
  lcd_create_char(0, lcd_box);
  lcd_create_char(1, lcd_left_arrow);
  lcd_create_char(2, lcd_right_arrow);
}

void lcd_clear() {
  for (int i=0; i<16; i++) lcd_buffer[i] = ' ';
  lcd_update_pos = 17;
}

void lcd_show_main() {
  lcd.setCursor(0, 0);
  lcd.print("IAMBINO  ");
  lcd_append_wpm();
}

void lcd_show_play() {
  lcd.setCursor( 0, 0 );
  lcd.print("PLAY ");
  lcd.print(message_get_index());
  lcd.print("   ");
  lcd_append_wpm();
}

void lcd_show_record() {
  lcd.setCursor(0, 0);
  lcd.print("REC ");
  lcd.print(message_get_index());
  lcd.print("    ");
  lcd_append_wpm();
}

void lcd_append_wpm() {
  if (cfg_get_speed() < 100) lcd.print(" ");
  if (cfg_get_speed() < 10) lcd.print(" ");
  lcd.print(cfg_get_speed(), 0);
  lcd.print(" WPM");
  lcd_update_pos = 17;
}

void lcd_show_settings() {
  lcd.setCursor(0, 0);
  lcd.print(F("\001   SETTINGS   \002"));
}

void lcd_write(uint16_t c) {
  if (c&0xFF00) {
    lcd_buffer[lcd_pos++&0x0F] = '{';
    lcd_buffer[lcd_pos++&0x0F] = c>>8;
    lcd_buffer[lcd_pos++&0x0F] = c;
    lcd_buffer[lcd_pos++&0x0F] = '}';
  } else {
    lcd_buffer[lcd_pos++&0x0F] = c;
  }
  lcd_update_pos = 17;
}

void lcd_loop() {
  if (lcd_update_pos == 17) {
    lcd.setCursor(0, 1);
    lcd_update_pos = 0;
  } else if (lcd_update_pos < 16) {
    lcd.write(lcd_buffer[(lcd_pos+lcd_update_pos)&0x0F]);
    lcd_update_pos++;
  }
}


void cfg_set_backlight(uint8_t level) {
  cfg.backlight = level;
  lcd_set_backlight(level);
}

void cfg_backlight(uint8_t button) {
  if (button == BUTTON_NONE) {
    cfg_set_backlight(6);
    return;
  }
  
  button_fast(true);
  
  if (button & BUTTON_UP) {
    if (cfg.backlight < CFG_BACKLIGHT_MAX) {
      cfg_set_backlight(cfg.backlight+1);
    }
  }
  if (button & BUTTON_DOWN) {
    if (cfg.backlight > 0) {
      cfg_set_backlight(cfg.backlight-1);
    }
  }
  
  lcd.setCursor( 0, 1 );
  lcd.print(F("BACKLIGHT: "));
  lcd.print(cfg.backlight);
  lcd.print(LCD_CLEAR_8);
}

