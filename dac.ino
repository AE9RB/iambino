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

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define DAC_STATE_ATTACK 0
#define DAC_STATE_SUSTAIN 1
#define DAC_STATE_RELEASE 2
#define DAC_STATE_IDLE 3

float dac_tones[] = {
   220.000, // A3
   246.942, // B3
   261.626, // C4
   293.665, // D4
   329.628, // E4
   349.228, // F4
   391.995, // G4
   440.000, // A4
   493.883, // B4
   523.251, // C5
   587.330, // D5
   659.255, // E5
   698.456, // F5
   783.991, // G5
   880.000, // A5
   987.767, // B5
  1046.502, // C6
  1174.659, // D6
  1318.510, // E6
  1396.913, // F6
  1567.982, // G6
  1760.000, // A6
  1975.533, // B6
  2093.005, // C7
  2349.318, // D7
  2637.021, // E7
  2793.826, // F7
  3135.964, // G7
};

bool dac_playing = false;
bool dac_changed = false;
long dac_timer;
float dac_volume;
float dac_tone;
uint16_t dac_wav_atk[180];
uint16_t dac_wav_sus[80];
uint16_t dac_wav_rel[80];
uint8_t dac_wav_atk_len;
uint8_t dac_wav_sus_len;
uint8_t dac_wav_rel_len;

// Compute tone using: pow(e, j * 2 * PI * freq * time)
// kind: 0=attack 1=sustain 2=release
void dac_wavegen(uint8_t kind, uint8_t samples, uint8_t size, uint16_t data[]) {
  float f, signal_inc_r, signal_inc_i, signal_r, signal_i;
  float shape_inc_r, shape_inc_i, shape_r, shape_i;
  int i;
  f = M_PI * 2 * (1.0/samples);
  signal_inc_r = cos(f);
  signal_inc_i = sin(f);
  signal_r = 0.0;
  signal_i = -1.0;
  f = f / 4 / size;
  shape_inc_r = cos(f);
  shape_inc_i = sin(f);
  if (kind == 0) {
    shape_r = 0.0;
    shape_i = -1.0;
  } else {
    shape_r = 1.0;
    shape_i = 0.0;
  }
  for (i=(samples*size)-1; i>=0; i--) {
    f = signal_r;
    signal_r = signal_r * signal_inc_r - signal_i * signal_inc_i;
    signal_i = signal_i * signal_inc_r + f * signal_inc_i;
    if (kind != 1) {
      f = shape_r;
      shape_r = shape_r * shape_inc_r - shape_i * shape_inc_i;
      shape_i = shape_i * shape_inc_r + f * shape_inc_i;
      f = signal_r * shape_r;
    } else {
      f = signal_r;
    }
    data[i] = (int)(f * dac_volume * 32767) + 32768;
  }
}

// Play tone until millis mark
void dac_play(long mark) {
  dac_playing = true;
  dac_timer = mark;
}

void dac_loop(long mark) {
  uint8_t sine_samples;
  uint8_t attack_size;
  uint8_t release_size;
  
  if (dac_playing) {
    if(dac_timer - mark < 0) dac_playing = false;
  } 
  else if (dac_changed) {
    // Ensure buffers are large enough for all dac_tones.
    //   dac_wav_atk = max(sine_samples*attack_size)
    //   dac_wav_sus = max(sine_samples)
    //   dac_wav_rel = max(sine_samples*release_size)
    sine_samples = 12;
    while (sine_samples * dac_tone < 16000) sine_samples += 4;
    attack_size = 60 / sine_samples + 2;
    release_size = attack_size / 2;
    dac_wavegen(0, sine_samples, attack_size, dac_wav_atk);
    dac_wavegen(1, sine_samples, 1, dac_wav_sus);
    dac_wavegen(2, sine_samples, release_size, dac_wav_rel);
    dac_wav_atk_len = sine_samples * attack_size;
    dac_wav_sus_len = sine_samples;
    dac_wav_rel_len = sine_samples * release_size;
    OCR1A =  F_CPU / (dac_tone * sine_samples);
    dac_changed = false;
    dac_play(mark + 175000);
  }
}

void dac_setup(void) {
  // Set all pins for output
  sbi(DAC_CS_DDR, DAC_CS_BIT);
  sbi(DAC_SCK_DDR, DAC_SCK_BIT);
  sbi(DAC_SDI_DDR, DAC_SDI_BIT);

  // Arduino/wiring will disable interrupts when reading the 
  // millis or micros clock. Using LDAC will avoid the jitter.
  // Pin 10 is the only one compatible with LDAC because
  // Timer 1 is the only 16 bit PWM on an ATmega328.
  pinMode( 10, OUTPUT );
  TCCR1A = _BV(WGM10) | _BV(WGM11) | _BV(COM1B1);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);
  OCR1B = 255;

  // Setup initial tone and cancel beep
  dac_loop(0);
  dac_playing = false;

  // start ISR
  TIMSK1 |= _BV(OCIE1A);
}

inline void dac_sck_pulse() {
  sbi(DAC_SCK_PORT, DAC_SCK_BIT);
  cbi(DAC_SCK_PORT, DAC_SCK_BIT);
}

inline void dac_sdi_send(uint8_t data, uint8_t bit) {
  if(data & _BV(bit)) sbi(DAC_SDI_PORT, DAC_SDI_BIT);
  else cbi(DAC_SDI_PORT, DAC_SDI_BIT);
  dac_sck_pulse();
}

ISR(TIMER1_COMPA_vect) {
  static uint8_t pos = 0, dac_state = DAC_STATE_IDLE;
  uint8_t dh, dl;
  
  if (!pos) {
    if (dac_playing) {
      if (dac_state <= DAC_STATE_SUSTAIN) dac_state = DAC_STATE_SUSTAIN;
      else dac_state = DAC_STATE_ATTACK;
    } else {
      if (dac_state <= DAC_STATE_SUSTAIN) dac_state = DAC_STATE_RELEASE;
      else dac_state = DAC_STATE_IDLE;
    }
  }
  
  switch (dac_state) {
    case DAC_STATE_ATTACK:
      if (!pos) pos = dac_wav_atk_len;
      pos--;
      dh = dac_wav_atk[pos] >> 8;
      dl = dac_wav_atk[pos] & 0xFF;
      break;
    case DAC_STATE_SUSTAIN:
      if (!pos) pos = dac_wav_sus_len;
      pos--;
      dh = dac_wav_sus[pos] >> 8;
      dl = dac_wav_sus[pos] & 0xFF;
      break;
    case DAC_STATE_RELEASE:
      if (!pos) pos = dac_wav_rel_len;
      pos--;
      dh = dac_wav_rel[pos] >> 8;
      dl = dac_wav_rel[pos] & 0xFF;
      break;
    default: //DAC_STATE_IDLE
      dh = 32768 >> 8;
      dl = 32768 & 0xFF;
      break;
  }
  
  // begin data send
  cbi(DAC_CS_PORT, DAC_CS_BIT);
  // config bits
  cbi(DAC_SDI_PORT, DAC_SDI_BIT);
  dac_sck_pulse(); // write register
  dac_sck_pulse(); // BUF = unbuffered
  sbi(DAC_SDI_PORT, DAC_SDI_BIT);
  dac_sck_pulse(); // GA = 1X gain
  dac_sck_pulse(); // SHDN = active mode 
  // 12 data bits
  dac_sdi_send(dh, 7);
  dac_sdi_send(dh, 6);
  dac_sdi_send(dh, 5);
  dac_sdi_send(dh, 4);
  dac_sdi_send(dh, 3);
  dac_sdi_send(dh, 2);
  dac_sdi_send(dh, 1);
  dac_sdi_send(dh, 0);
  dac_sdi_send(dl, 7);
  dac_sdi_send(dl, 6);
  dac_sdi_send(dl, 5);
  dac_sdi_send(dl, 4);
  // end data send
  sbi(DAC_CS_PORT, DAC_CS_BIT);
}


float cfg_get_tone() {
  return dac_tone;
}

void cfg_set_tone(float newtone) {
  dac_tone = newtone;
  dac_changed = true;
  dac_playing = false;
}

void cfg_tone(uint8_t button) {
  int i;
  button_fast(true);

  if (button & BUTTON_UP) {
    for(i=0;i<(int)(sizeof(dac_tones)/sizeof(float));i++) {
      if (dac_tones[i] > dac_tone) {
        dac_tone = dac_tones[i];
        cfg_set_tone(dac_tone);
        break;
      }
    }
  }

  if (button & BUTTON_DOWN) {
    for(i=sizeof(dac_tones)/sizeof(float)-1;i>=0;i--) {
      if (dac_tones[i] < dac_tone) {
        dac_tone = dac_tones[i];
        cfg_set_tone(dac_tone);
        break;
      }
    }
  }

  lcd.setCursor( 0, 1 );
  lcd.print("TONE: ");
  lcd.print(dac_tone, 1);
  lcd.print(" Hz");
  lcd.print("  ");
}


uint8_t cfg_volume_level;

uint8_t cfg_get_volume() {
  return cfg_volume_level;
}

void cfg_set_volume(uint8_t volume) {
  cfg_volume_level = volume;
  dac_changed = true;
  dac_playing = false;
  dac_volume = 1 - log10(((float)CFG_VOLUME_MAX / 9 + CFG_VOLUME_MAX - volume) * 9 / CFG_VOLUME_MAX);
}

void cfg_volume(uint8_t button) {
  bool changed = false;
  button_fast(true);
  if (button & BUTTON_UP) {
    if (cfg_volume_level < CFG_VOLUME_MAX) {
      cfg_volume_level++;
      changed = true;
    }
  }
  if (button & BUTTON_DOWN) {
    if (cfg_volume_level > 0) {
      cfg_volume_level--;
      changed = true;
    }
  }
  
  lcd.setCursor( 0, 1 );
  lcd.print("VOLUME: ");
  lcd.print(cfg_volume_level);
  lcd.print(LCD_CLEAR_8);
  if (changed) cfg_set_volume(cfg_volume_level);
}
