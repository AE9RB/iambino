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

#define BUTTON_DEBOUNCE_DN   2000  // 2ms
#define BUTTON_DEBOUNCE_UP   50000 // 50ms
#define BUTTON_REPEAT_DELAY  400000
#define BUTTON_REPEAT_EACH   150000
#define BUTTON_ADC_RIGHT     0
#define BUTTON_ADC_UP        145
#define BUTTON_ADC_DOWN      329
#define BUTTON_ADC_LEFT      505
#define BUTTON_ADC_SELECT    741
#define BUTTON_ADC_WINDOW    10


bool button_repeat_fast;

void button_fast(bool is_fast) {
  button_repeat_fast = is_fast;
}

uint8_t button_read(long mark) {
  static uint8_t state = 0, eat, pressed;
  static long debounce, repeat;
  
  uint8_t button = BUTTON_NONE;
  int adc;
  
  adc = analogRead( BUTTON_APIN );
  if( adc < ( BUTTON_ADC_RIGHT + BUTTON_ADC_WINDOW ) )
  {
    button = BUTTON_SELECT;
  }
  else if(   adc >= ( BUTTON_ADC_UP - BUTTON_ADC_WINDOW )
          && adc <= ( BUTTON_ADC_UP + BUTTON_ADC_WINDOW ) )
  {
    button = BUTTON_RIGHT;
  }
  else if(   adc >= ( BUTTON_ADC_DOWN - BUTTON_ADC_WINDOW )
          && adc <= ( BUTTON_ADC_DOWN + BUTTON_ADC_WINDOW ) )
  {
    button = BUTTON_UP;
  }
  else if(   adc >= ( BUTTON_ADC_LEFT - BUTTON_ADC_WINDOW )
          && adc <= ( BUTTON_ADC_LEFT + BUTTON_ADC_WINDOW ) )
  {
    button = BUTTON_DOWN;
  }
  else if(   adc >= ( BUTTON_ADC_SELECT - BUTTON_ADC_WINDOW )
          && adc <= ( BUTTON_ADC_SELECT + BUTTON_ADC_WINDOW ) )
  {
    button = BUTTON_LEFT;
  }
  
  switch(state) {
  case 0:
    if (button) {
      debounce = mark + BUTTON_DEBOUNCE_DN;
      state = 1;
      eat = 1;
    }
    break;
  case 1:
    if (mark - debounce >= 0) {
      if (button) {
        state = 2;
        repeat = mark + BUTTON_REPEAT_DELAY;
        pressed = button;
        return button;
      }
      state = 0;
    }
    break;
  case 2:
    if (button) debounce = mark + BUTTON_DEBOUNCE_UP;
    else if (mark - debounce >= 0) {
      state = 0;
      return pressed | BUTTON_RELEASE;
    }
    if (mark - repeat >= 0) {
      repeat = mark + BUTTON_REPEAT_EACH;
      eat ^= 1;
      if (!eat || ((button & BUTTON_UP || button & BUTTON_DOWN) && button_repeat_fast))
        return pressed | BUTTON_REPEAT;
    }
    break;
  }
  
  return BUTTON_NONE;
}


// TODO start at 512 and save these in EEPROM
// prototype hardware is full range so this is ok for now
int button_pot_min = 0;
int button_pot_max = 1023;

bool button_pot() {
  static unsigned int prev_speed;
  unsigned int seg, pos, mod, speed, dz;
  int i = analogRead(POT_APIN);
  
  if (i > button_pot_max) {
    button_pot_max = i;
    cfg_set_speed(0);
  }
  if (i < button_pot_min) {
    button_pot_min = i;
    cfg_set_speed(0);
  }
  
  seg = (1 + button_pot_max - button_pot_min) / (1 + cfg.speed_max - cfg.speed_min);
  dz = seg / 4;
  pos = i - button_pot_min;
  mod = pos % seg;
  speed = pos / seg + cfg.speed_min;
  if (speed > cfg.speed_max) speed = cfg.speed_max;
  
  if (cfg_get_speed()==0 || (mod > dz && seg-mod > dz && prev_speed != speed)) {
    cfg_set_speed(speed);
    prev_speed = speed;
    return true;
  }
  return false;
}