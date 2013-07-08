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


// Debounce straight key both up and down.
#define KEY_DEBOUNCE_SRAIGHT 8000

// Start looking for key input 2ms early in case the key is bouncing
// up at the moment we're suppose to start a new dit or dah.
#define KEY_DEBOUNCE_IAMBIC 2000

uint16_t cfg_speed_min_wpm;
uint16_t cfg_speed_max_wpm;
float cfg_speed_wpm;
long cfg_speed_micros;

float cfg_get_speed() {
  return cfg_speed_wpm;
}

long cfg_get_speed_micros() {
  return cfg_speed_micros;
}

void cfg_set_speed(float wpm) {
  cfg_speed_wpm = wpm;
  cfg_speed_micros = 1200000 / wpm;
}

void cfg_speed(uint8_t button) {
  button_fast(true);
  
  if (button & BUTTON_UP)
    if (cfg_speed_wpm < cfg_speed_max_wpm)
      cfg_set_speed(cfg_speed_wpm + 1);

  if (button & BUTTON_DOWN)
    if (cfg_speed_wpm > cfg_speed_min_wpm)
      cfg_set_speed(cfg_speed_wpm - 1);
  
  lcd.setCursor( 0, 1 );
  lcd.print("SPEED: ");
  lcd.print((int)cfg_speed_wpm);
  lcd.print(" WPM");
  lcd.print(LCD_CLEAR_8);
}


uint16_t cfg_get_speed_min() {
  return cfg_speed_min_wpm;
}

void cfg_set_speed_min(uint16_t wpm) {
  cfg_speed_min_wpm = wpm;
}

void cfg_speed_min(uint8_t button) {
  button_fast(true);
  
  if (button & BUTTON_UP)
    if (cfg_speed_min_wpm < CFG_SPEED_MAX) {
      cfg_speed_min_wpm += 1;
      cfg_speed_wpm = 0;
    }

  if (button & BUTTON_DOWN)
    if (cfg_speed_min_wpm > CFG_SPEED_MIN) {
      cfg_speed_min_wpm -= 1;
      cfg_speed_wpm = 0;
    }
      
  if (cfg_speed_min_wpm > cfg_speed_max_wpm)
    cfg_speed_max_wpm = cfg_speed_min_wpm;
  
  lcd.setCursor( 0, 1 );
  lcd.print("SPD MIN: ");
  lcd.print((int)cfg_speed_min_wpm);
  lcd.print(" WPM");
  lcd.print(LCD_CLEAR_8);
}


uint16_t cfg_get_speed_max() {
  return cfg_speed_max_wpm;
}

void cfg_set_speed_max(uint16_t wpm) {
  cfg_speed_max_wpm = wpm;
}

void cfg_speed_max(uint8_t button) {
  button_fast(true);
  
  if (button & BUTTON_UP)
    if (cfg_speed_max_wpm < CFG_SPEED_MAX) {
      cfg_speed_max_wpm += 1;
      cfg_speed_wpm = 0;
    }

  if (button & BUTTON_DOWN)
    if (cfg_speed_max_wpm > CFG_SPEED_MIN) {
      cfg_speed_max_wpm -= 1;
      cfg_speed_wpm = 0;
    }
      
  if (cfg_speed_max_wpm < cfg_speed_min_wpm)
    cfg_speed_min_wpm = cfg_speed_max_wpm;
  
  lcd.setCursor( 0, 1 );
  lcd.print("SPD MAX: ");
  lcd.print((int)cfg_speed_max_wpm);
  lcd.print(" WPM");
  lcd.print(LCD_CLEAR_8);
}


int8_t cfg_mode_type;
const char *cfg_mode_text[] = {
  "IAMBIC",
  "ULTIMATIC",
  "BUG",
  "STRAIGHT"
};

int8_t cfg_get_mode() {
  return cfg_mode_type;
}

void cfg_set_mode(int8_t type) {
  cfg_mode_type = type;
}

void cfg_mode(uint8_t button) {
  if (button & BUTTON_UP) cfg_mode_type++;
  if (button & BUTTON_DOWN) cfg_mode_type--;

  if (cfg_mode_type < 0) cfg_mode_type = sizeof(cfg_mode_text)/sizeof(char*)-1;
  if (cfg_mode_type >= (int8_t)(sizeof(cfg_mode_text)/sizeof(char*))) cfg_mode_type = 0;
  
  lcd.setCursor( 0, 1 );
  lcd.print("MODE: ");
  lcd.print(cfg_mode_text[cfg_mode_type]);
  lcd.print(LCD_CLEAR_8);
}


int8_t cfg_memory_type;
const char *cfg_memory_text[] = {
  "TYPE A",
  "DASH",
  "DOT",
  "TYPE B",
};

int8_t cfg_get_memory() {
  return cfg_memory_type;
}

void cfg_set_memory(int8_t type) {
  cfg_memory_type = type;
}

void cfg_memory(uint8_t button) {
  if (button & BUTTON_UP) cfg_memory_type--;
  if (button & BUTTON_DOWN) cfg_memory_type++;

  if (cfg_memory_type < 0) cfg_memory_type = sizeof(cfg_memory_text)/sizeof(char*)-1;
  if (cfg_memory_type >= (int8_t)(sizeof(cfg_memory_text)/sizeof(char*))) cfg_memory_type = 0;
  
  lcd.setCursor( 0, 1 );
  lcd.print("MEMORY: ");
  lcd.print(cfg_memory_text[cfg_memory_type]);
  lcd.print(LCD_CLEAR_8);
}


uint8_t cfg_lag_ms;

uint8_t cfg_get_lag() {
  return cfg_lag_ms;
}

void cfg_set_lag(uint8_t lag) {
  cfg_lag_ms = lag;
}

void cfg_lag(uint8_t button) {
  button_fast(true);
  
  if (button & BUTTON_UP && cfg_lag_ms < CFG_LAG_MAX) cfg_lag_ms++;
  if (button & BUTTON_DOWN && cfg_lag_ms > 0) cfg_lag_ms--;

  lcd.setCursor( 0, 1 );
  lcd.print("TX LAG: ");
  lcd.print(cfg_lag_ms);
  lcd.print(" ms");
  lcd.print(LCD_CLEAR_8);
}


float cfg_weight_pct;

float cfg_get_weight() {
  return cfg_weight_pct;
}

void cfg_set_weight(float weight) {
  cfg_weight_pct = weight;
}

void cfg_weight(uint8_t button) {
  button_fast(true);
  if (button & BUTTON_UP && cfg_weight_pct < 0.49 + CFG_WEIGHT_DIST) cfg_weight_pct += 0.01;
  if (button & BUTTON_DOWN && cfg_weight_pct > 0.51 - CFG_WEIGHT_DIST) cfg_weight_pct -= 0.01;

  lcd.setCursor( 0, 1 );
  lcd.print("WEIGHTING: ");
  lcd.print(cfg_weight_pct * 100, 0);
  lcd.print("%");
  lcd.print(LCD_CLEAR_8);
}


int8_t cfg_spacing_type;
const char *cfg_spacing_text[] = {
  "NONE",
  "ELEMENT",
  "CHAR",
  "WORD"
};

int8_t cfg_get_spacing() {
  return cfg_spacing_type;
}

void cfg_set_spacing(int8_t spacing) {
  cfg_spacing_type = spacing;
}

void cfg_spacing(uint8_t button) {
  if (button & BUTTON_UP) cfg_spacing_type++;
  if (button & BUTTON_DOWN) cfg_spacing_type--;

  if (cfg_spacing_type < 0) cfg_spacing_type = sizeof(cfg_spacing_text)/sizeof(char*)-1;
  if (cfg_spacing_type >= (int8_t)(sizeof(cfg_spacing_text)/sizeof(char*))) cfg_spacing_type = 0;
  
  lcd.setCursor( 0, 1 );
  lcd.print("SPACING: ");
  lcd.print(cfg_spacing_text[cfg_spacing_type]);
  lcd.print(LCD_CLEAR_8);
}


uint8_t cfg_paddle_type;
const char *cfg_paddle_text[] = {
  "NORMAL",
  "REVERSE"
};

uint8_t cfg_get_paddle() {
  return cfg_paddle_type;
}

void cfg_set_paddle(uint8_t paddle) {
  cfg_paddle_type = paddle;
}

void cfg_paddle(uint8_t button) {
  if ((button & BUTTON_UP) || (button & BUTTON_DOWN)) {
    cfg_paddle_type++;
    if (cfg_paddle_type > 1) cfg_paddle_type = 0;
  }
  
  lcd.setCursor( 0, 1 );
  lcd.print("PADDLE: ");
  lcd.print(cfg_paddle_text[cfg_paddle_type]);
  lcd.print(LCD_CLEAR_8);
}


uint8_t key_loop(long mark) {
  static uint8_t last, spacing=2, ultimatic, state=3, staged=0, mcode=0x80;
  static long read_after, start_after;
  uint8_t k0, k1, ret = 0;
  long i;
  
  if (cfg_mode_type == CFG_MODE_STRAIGHT) {
    k1 = digitalRead(KEY_0);
    k0 = 1;
  } else if (cfg_paddle_type == CFG_PADDLE_NORMAL) {
    k0 = digitalRead(KEY_0);
    k1 = digitalRead(KEY_1);
  } else {
    k1 = digitalRead(KEY_0);
    k0 = digitalRead(KEY_1);
  }
  
  switch(state) {
  case 1: // waiting until ready for read
    if (cfg_spacing_type == CFG_SPACING_NONE)
      if ((!k0 && last == DIT) || (!k1 && last == DAH))
        read_after = mark + KEY_DEBOUNCE_IAMBIC;
    if (read_after - mark < 0) state = 2;
    break;
  case 2: // waiting and reading
    if (start_after - mark < 0) state = 3;
    if (spacing < 4) break;
    //nobreak;
  case 3: // idle, spacing
    if (start_after - mark < 0) {
      switch (spacing) {
      case 0:
      case 2:
      case 3:
        break;
      case 1:
        ret = mcode;
        mcode=0x80;
        if (cfg_spacing_type >= CFG_SPACING_CHAR) state = 2;
        break;
      case 4:
        ret = mcode;
        //nobreak
      case 5:
      case 6:
        if (cfg_spacing_type >= CFG_SPACING_WORD) state = 2;
        break;
      }
      if (spacing < 7) spacing += 1;
      if (cfg_mode_type == CFG_MODE_BUG) state = 3;
      start_after += DIT * cfg_speed_micros;
    }
    break;
  case 4: // debouncing straight/bug down
    if (start_after - mark < 0) state = 5;
    break;
  case 5: // holding straight/bug
    break;
  case 6: // debouncing straight/bug up
    if (read_after - mark < 0) {
      state = 3;
      staged = 0;
      start_after = mark + DIT * cfg_speed_micros;
      spacing = 0;
      if (mcode & 0x01) {
        mcode = 0xFF;
      } else {
        mcode >>= 1;
        mcode |= 0x80;
      }
    }
    break;
  }

  if (cfg_mode_type == CFG_MODE_STRAIGHT || cfg_mode_type == CFG_MODE_BUG) {
    if (!k1) {
      i = mark + KEY_DEBOUNCE_SRAIGHT;
      if (state < 4) {
        state = 4;
        digitalWrite(TX_PIN, HIGH);
        start_after = i;
      }
      if (state < 6) {
        read_after = i;
        dac_play(i);
      }
      last = DAH;
      staged = 0;
    } 
    else if (state == 5) {
      if (staged == DIT) {
        state = 3;
      } else {
        state = 6;
        dac_play(mark);
        digitalWrite(TX_PIN, LOW);
      }
    }
  }
  
  if (!staged) {
    if (state > 1) {
      if (!k0 && !k1) {
        if (ultimatic && cfg_mode_type == CFG_MODE_ULTIMATIC) staged = last;
        else if (last == DIT) staged = DAH;
        else staged = DIT;
        ultimatic = 1;
      } else {
        if (!k0) staged = DIT;
        if (!k1) staged = DAH;
        ultimatic = 0;
      }
    }
    else if (!ultimatic || cfg_mode_type != CFG_MODE_ULTIMATIC) {
      if (!k0 && (last == DAH || spacing > 0)) {
        if (cfg_memory_type & CFG_MEMORY_DIT) {
          staged = DIT;
          ultimatic = 1;
        }
      }
      if (!k1 && (last == DIT || spacing > 0)) {
        if (cfg_memory_type & CFG_MEMORY_DAH) {
          staged = DAH;
          ultimatic = 1;
        }
      }
    }
  }
  
  if (state == 3 && staged) {
    i = mark + (long)staged * cfg_speed_micros;
    i += DIT * cfg_speed_micros * (cfg_weight_pct * 2 - 1);
    dac_play(i);
    read_after = start_after = i + (long)cfg_lag_ms * 1000;
    tx_send(start_after);
    i += DIT * cfg_speed_micros * (2.0 - cfg_weight_pct * 2);
    if (cfg_spacing_type >= CFG_SPACING_EL) {
      read_after = i - KEY_DEBOUNCE_IAMBIC;
      start_after = i;
    }
    spacing = 0;
    if (mcode & 0x01) {
      if (mcode != 0x01 || staged==DAH) mcode = 0xFF;
    } else {
      mcode >>= 1;
      if (staged==DAH) mcode |= 0x80;
    }
    last = staged;
    staged = 0;
    state = 1;
  }

  if (cfg_mode_type == CFG_MODE_STRAIGHT) return 0;
  return ret;
}