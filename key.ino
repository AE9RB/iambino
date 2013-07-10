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

// Debounce straight key both up and down.
#define KEY_DEBOUNCE_SRAIGHT 8000

// Start looking for key input 2ms early in case the key is bouncing
// up at the moment we're suppose to start a new dit or dah.
#define KEY_DEBOUNCE_IAMBIC 2000

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
  if (button == BUTTON_NONE) return;
  
  button_fast(true);
  if (button & BUTTON_UP)
    if (cfg_speed_wpm < CFG_SPEED_MAX)
      cfg_set_speed(cfg_speed_wpm + 1);
  if (button & BUTTON_DOWN)
    if (cfg_speed_wpm > CFG_SPEED_MIN)
      cfg_set_speed(cfg_speed_wpm - 1);
  
  lcd.setCursor( 0, 1 );
  lcd.print(F("SPEED: "));
  lcd.print((int)cfg_speed_wpm);
  lcd.print(" WPM");
  lcd.print(LCD_CLEAR_8);
}


void cfg_speed_min(uint8_t button) {
  if (button == BUTTON_NONE) {
    cfg.speed_min = 5;
    return;
  }
  
  button_fast(true);
  if (button & BUTTON_UP)
    if (cfg.speed_min < CFG_SPEED_MAX) {
      cfg.speed_min += 1;
      cfg_speed_wpm = 0;
    }
  if (button & BUTTON_DOWN)
    if (cfg.speed_min > CFG_SPEED_MIN) {
      cfg.speed_min -= 1;
      cfg_speed_wpm = 0;
    }
      
  if (cfg.speed_min > cfg.speed_max)
    cfg.speed_max = cfg.speed_min;
  
  lcd.setCursor( 0, 1 );
  lcd.print(F("SPD MIN: "));
  lcd.print(cfg.speed_min);
  lcd.print(" WPM");
  lcd.print(LCD_CLEAR_8);
}


void cfg_speed_max(uint8_t button) {
  if (button == BUTTON_NONE) {
    cfg.speed_max = 40;
    return;
  }
  
  button_fast(true);
  if (button & BUTTON_UP)
    if (cfg.speed_max < CFG_SPEED_MAX) {
      cfg.speed_max += 1;
      cfg_speed_wpm = 0;
    }
  if (button & BUTTON_DOWN)
    if (cfg.speed_max > CFG_SPEED_MIN) {
      cfg.speed_max -= 1;
      cfg_speed_wpm = 0;
    }
      
  if (cfg.speed_max < cfg.speed_min)
    cfg.speed_min = cfg.speed_max;
  
  lcd.setCursor( 0, 1 );
  lcd.print(F("SPD MAX: "));
  lcd.print(cfg.speed_max);
  lcd.print(" WPM");
  lcd.print(LCD_CLEAR_8);
}


const char *cfg_mode_text[] = {
  "IAMBIC",
  "ULTIMATIC",
  "BUG",
  "STRAIGHT"
};

void cfg_mode(uint8_t button) {
  if (button == BUTTON_NONE) {
    cfg.mode = 0;
    return;
  }
  
  if (button & BUTTON_UP) cfg.mode++;
  if (button & BUTTON_DOWN) cfg.mode--;

  if (cfg.mode < 0) cfg.mode = sizeof(cfg_mode_text)/sizeof(char*)-1;
  if (cfg.mode >= (int8_t)(sizeof(cfg_mode_text)/sizeof(char*))) cfg.mode = 0;
  
  lcd.setCursor( 0, 1 );
  lcd.print(F("MODE: "));
  lcd.print(cfg_mode_text[cfg.mode]);
  lcd.print(LCD_CLEAR_8);
}


const char *cfg_memory_text[] = {
  "TYPE A",
  "DASH",
  "DOT",
  "TYPE B",
};

void cfg_memory(uint8_t button) {
  if (button == BUTTON_NONE) {
    cfg.memory = 3;
    return;
  }
  
  if (button & BUTTON_UP) cfg.memory--;
  if (button & BUTTON_DOWN) cfg.memory++;
  if (cfg.memory < 0) cfg.memory = sizeof(cfg_memory_text)/sizeof(char*)-1;
  if (cfg.memory >= (int8_t)(sizeof(cfg_memory_text)/sizeof(char*))) cfg.memory = 0;
  
  lcd.setCursor( 0, 1 );
  lcd.print(F("MEMORY: "));
  lcd.print(cfg_memory_text[cfg.memory]);
  lcd.print(LCD_CLEAR_8);
}


void cfg_lag(uint8_t button) {
  if (button == BUTTON_NONE) {
    cfg.lag = 0;
    return;
  }

  button_fast(true);  
  if (button & BUTTON_UP && cfg.lag < CFG_LAG_MAX) cfg.lag++;
  if (button & BUTTON_DOWN && cfg.lag > 0) cfg.lag--;

  lcd.setCursor( 0, 1 );
  lcd.print(F("TX LAG: "));
  lcd.print(cfg.lag);
  lcd.print(F(" ms"));
  lcd.print(LCD_CLEAR_8);
}


void cfg_weight(uint8_t button) {  
  if (button == BUTTON_NONE) {
    cfg.weight = 0.50;
    return;
  }
  
  button_fast(true);
  if (button & BUTTON_UP && cfg.weight < 0.49 + CFG_WEIGHT_DIST) cfg.weight += 0.01;
  if (button & BUTTON_DOWN && cfg.weight > 0.51 - CFG_WEIGHT_DIST) cfg.weight -= 0.01;

  lcd.setCursor( 0, 1 );
  lcd.print(F("WEIGHTING: "));
  lcd.print(cfg.weight * 100, 0);
  lcd.print("%");
  lcd.print(LCD_CLEAR_8);
}


const char *cfg_spacing_text[] = {
  "NONE",
  "ELEMENT",
  "CHAR",
  "WORD"
};

void cfg_spacing(uint8_t button) {
  if (button == BUTTON_NONE) {
    cfg.spacing = 1;
    return;
  }
  
  if (button & BUTTON_UP) cfg.spacing++;
  if (button & BUTTON_DOWN) cfg.spacing--;
  if (cfg.spacing < 0) cfg.spacing = sizeof(cfg_spacing_text)/sizeof(char*)-1;
  if (cfg.spacing >= (int8_t)(sizeof(cfg_spacing_text)/sizeof(char*))) cfg.spacing = 0;
  
  lcd.setCursor( 0, 1 );
  lcd.print(F("SPACING: "));
  lcd.print(cfg_spacing_text[cfg.spacing]);
  lcd.print(LCD_CLEAR_8);
}


const char *cfg_paddle_text[] = {
  "NORMAL",
  "REVERSE"
};

void cfg_paddle(uint8_t button) {
  if (button == BUTTON_NONE) {
    cfg.paddle = 0;
    return;
  }

  if ((button & BUTTON_UP) || (button & BUTTON_DOWN)) {
    cfg.paddle++;
    if (cfg.paddle > 1) cfg.paddle = 0;
  }
  
  lcd.setCursor( 0, 1 );
  lcd.print(F("PADDLE: "));
  lcd.print(cfg_paddle_text[cfg.paddle]);
  lcd.print(LCD_CLEAR_8);
}


uint8_t key_read() {
  uint8_t k0, k1;
  k0 = digitalRead(KEY_0) ^ 1;
  k1 = digitalRead(KEY_1) ^ 1;
  if (cfg.mode == CFG_MODE_STRAIGHT) {
    k0 <<= 1;
    k1 = 0;
  } else if (cfg.paddle == CFG_PADDLE_NORMAL) {
    k1 <<= 1;
  } else {
    k0 <<= 1;
  }
  return (k0|k1);
}


uint8_t key_loop(long mark) {
  static uint8_t last, spacing=2, ultimatic, state=3, staged=0, mcode=0x80;
  static long read_after, start_after;
  uint8_t k0, k1, ret = 0;
  long i;
  
  k0 = key_read();
  k1 = k0 & 2;
  k0 = k0 & 1;
  
  switch(state) {
  case 1: // waiting until ready for read
    if (cfg.spacing == CFG_SPACING_NONE)
      if ((k0 && last == DIT) || (k1 && last == DAH))
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
        if (cfg.spacing >= CFG_SPACING_CHAR) state = 2;
        break;
      case 4:
        ret = mcode;
        //nobreak
      case 5:
      case 6:
        if (cfg.spacing >= CFG_SPACING_WORD) state = 2;
        break;
      }
      if (spacing < 7) spacing += 1;
      if (cfg.mode == CFG_MODE_BUG) state = 3;
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

  if (cfg.mode == CFG_MODE_STRAIGHT || cfg.mode == CFG_MODE_BUG) {
    if (k1) {
      i = mark + KEY_DEBOUNCE_SRAIGHT;
      if (state < 4) {
        state = 4;
        start_after = i;
      }
      if (state < 6) {
        read_after = i;
        dac_play(i);
        tx_send(i);
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
        tx_send(mark);
      }
    }
  } else {
    if (state > 3) state = 6;
  }
  
  if (!staged) {
    if (state > 1) {
      if (k0 && k1) {
        if (ultimatic && cfg.mode == CFG_MODE_ULTIMATIC) staged = last;
        else if (last == DIT) staged = DAH;
        else staged = DIT;
        ultimatic = 1;
      } else {
        if (k0) staged = DIT;
        if (k1) staged = DAH;
        ultimatic = 0;
      }
    }
    else if (!ultimatic || cfg.mode != CFG_MODE_ULTIMATIC) {
      if (k0 && (last == DAH || spacing > 0)) {
        if (cfg.memory & CFG_MEMORY_DIT) {
          staged = DIT;
          ultimatic = 1;
        }
      }
      if (k1 && (last == DIT || spacing > 0)) {
        if (cfg.memory & CFG_MEMORY_DAH) {
          staged = DAH;
          ultimatic = 1;
        }
      }
    }
  }
  
  if (state == 3 && staged) {
    i = mark + (long)staged * cfg_speed_micros;
    i += DIT * cfg_speed_micros * (cfg.weight * 2 - 1);
    dac_play(i);
    read_after = start_after = i + (long)cfg.lag * 1000;
    tx_send(start_after);
    i += DIT * cfg_speed_micros * (2.0 - cfg.weight * 2);
    if (cfg.spacing >= CFG_SPACING_EL) {
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

  if (cfg.mode == CFG_MODE_STRAIGHT) return 0;
  return ret;
}