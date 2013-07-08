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

#define MCODE_L(x) (((x&0x0000000FUL)?1:0)\
  +((x&0x000000F0UL)?1:0)\
  +((x&0x00000F00UL)?1:0)\
  +((x&0x0000F000UL)?1:0)\
  +((x&0x000F0000UL)?1:0)\
  +((x&0x00F00000UL)?1:0)\
  +((x&0x0F000000UL)?1:0))
#define MCODE_C(x) (((x&0x00000002UL)?128:0)\
  +((x&0x00000020UL)?64:0)\
  +((x&0x00000200UL)?32:0)\
  +((x&0x00002000UL)?16:0)\
  +((x&0x00020000UL)?8:0)\
  +((x&0x00200000UL)?4:0)\
  +((x&0x02000000UL)?2:0))
#define MCODE(d)((uint8_t) (d==0||d==0x80)?d:MCODE_C(0x##d##U)\
  +(1<<(7-MCODE_L(0x##d##U))))

const uint8_t MCODES[] = {
    /* 0x20   */ MCODE(128),
    /* 0x21 ! */ MCODE(0),
    /* 0x22 " */ MCODE(131131),
    /* 0x23 # */ MCODE(0),
    /* 0x24 $ */ MCODE(0),
    /* 0x25 % */ MCODE(0),
    /* 0x26 & */ MCODE(0),
    /* 0x27 ' */ MCODE(133331),
    /* 0x28 ( */ MCODE(31331),
    /* 0x29 ) */ MCODE(313313),
    /* 0x2A * */ MCODE(0),
    /* 0x2B + */ MCODE(13131),
    /* 0x2C , */ MCODE(331133),
    /* 0x2D - */ MCODE(311113),
    /* 0x2E . */ MCODE(131313),
    /* 0x2F / */ MCODE(31131),
    /* 0x30 0 */ MCODE(33333),
    /* 0x31 1 */ MCODE(13333),
    /* 0x32 2 */ MCODE(11333),
    /* 0x33 3 */ MCODE(11133),
    /* 0x34 4 */ MCODE(11113),
    /* 0x35 5 */ MCODE(11111),
    /* 0x36 6 */ MCODE(31111),
    /* 0x37 7 */ MCODE(33111),
    /* 0x38 8 */ MCODE(33311),
    /* 0x39 9 */ MCODE(33331),
    /* 0x3A : */ MCODE(333111),
    /* 0x3B ; */ MCODE(0),
    /* 0x3C < */ MCODE(0),
    /* 0x3D = */ MCODE(31113),
    /* 0x3E > */ MCODE(0),
    /* 0x3F ? */ MCODE(113311),
    /* 0x40 @ */ MCODE(133131),
    /* 0x41 A */ MCODE(13),
    /* 0x42 B */ MCODE(3111),
    /* 0x43 C */ MCODE(3131),
    /* 0x44 D */ MCODE(311),
    /* 0x45 E */ MCODE(1),
    /* 0x46 F */ MCODE(1131),
    /* 0x47 G */ MCODE(331),
    /* 0x48 H */ MCODE(1111),
    /* 0x49 I */ MCODE(11),
    /* 0x4A J */ MCODE(1333),
    /* 0x4B K */ MCODE(313),
    /* 0x4C L */ MCODE(1311),
    /* 0x4D M */ MCODE(33),
    /* 0x4E N */ MCODE(31),
    /* 0x4F O */ MCODE(333),
    /* 0x50 P */ MCODE(1331),
    /* 0x51 Q */ MCODE(3313),
    /* 0x52 R */ MCODE(131),
    /* 0x53 S */ MCODE(111),
    /* 0x54 T */ MCODE(3),
    /* 0x55 U */ MCODE(113),
    /* 0x56 V */ MCODE(1113),
    /* 0x57 W */ MCODE(133),
    /* 0x58 X */ MCODE(3113),
    /* 0x59 Y */ MCODE(3133),
    /* 0x5A Z */ MCODE(3311),
};

uint8_t morse_mcode_for(uint8_t c) {
  if (c >= 0x20 && c < 0x20 + sizeof(MCODES)) {
    return MCODES[c-0x20];
  }
  return 0;
}

uint8_t morse_char_for(uint8_t mcode) {
  for (uint8_t i=0; i < sizeof(MCODES); i++) {
    if (mcode == MCODES[i]) return i + 0x20;
  }
  return 0;
}
