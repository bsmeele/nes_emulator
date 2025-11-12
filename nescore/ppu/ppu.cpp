#include "ppu/ppu.h"

#include <algorithm>

PPU::PPU(Bus* bus) {
  this->bus = bus;

  this->PPUCTRL = 0x00;
  this->PPUMASK = 0x00;
  this->PPUSTATUS = 0b1010000;  // PPUSTATUS is on power up as follows: +0+x xxxx, where + is often set. For simplicity, + is taken as 1 and x is taken as 0
  this->OAMADDR = 0x00;
  this->PPUSCROLL = 0x00;
  this->PPUADDR = 0x00;
  this->PPUDATA = 0x00;

  // Power up state for interanl registers is not specified
  this->v = 0;
  this->t = 0;
  this->x = 0;
  this->w = false;

  // Power up state for internal lathes is not specified
  this->upper_shift = 0x0000;
  this->lower_shift = 0x0000;
  this->attr_1 = false;
  this->attr_2 = false;
  this->attr_shift_1 = 0x00;
  this->attr_shift_2 = 0x00;
  
  // OAM - 0x00 (the actual power up value is unspecified, presumably random)
  std::fill(std::begin(this->OAM), std::end(this->OAM), 0xFF);
  // Secondary OAM - 0x00, alternatively 0xFF since that is the value the sprite evaluation routine uses for cleared entries (te actual power up value is unspecified, presumably random)
  std::fill(std::begin(this->secondary_OAM), std::end(this->secondary_OAM), 0xFF);

  // Sprite units - default

  this->scanline = 0;
  this->dot = 0;
}

void PPU::reset() {
  this->PPUCTRL = 0x00;
  this->PPUMASK = 0x00;
  // PPUSTATUS is reset as follows: U??x xxxx, where U is unchanged, ? is unkown, and x is irrelevant. For simplicity, it is implemented here as completely unchanged
  // OAMADDR remains unchanged on reset
  this->PPUSCROLL = 0x00;
  // PUADDR remains unchanged on reset
  this->PPUDATA = 0x00;

  // Reset state for internal registers and internal latches are unspecified

  this->scanline = 0;
  this->dot = 0;
}

uint8_t PPU::gen_pixel() {
  uint8_t bg = 0x00;
  bg |= 0x0001 & (this->lower_shift >> this->x) & 0x0001;
  bg |= ((this->upper_shift >> this->x) & 0x0001) << 1;
  bg |= ((this->attr_shift_1 >> this->x) & 0x01) << 2;
  bg |= ((this->attr_shift_2 >> this->x) & 0x01) << 3;

  this->upper_shift >>= 1;
  this->lower_shift >>= 1;
  this->lower_shift |= 0x8000;

  this->attr_shift_1 >>= 1;
  attr_shift_1 |= 0x80 * attr_1;
  this->attr_shift_2 >>= 1;
  attr_shift_2 |= 0x80 * attr_2;

  uint8_t sprite = 0x00;
  for (size_t i = 0; i < 8; i++) {
    uint8_t p = this->sprite_units[7 - i].tick();
    
    if ((p & 0x03) != 0) {
      sprite = p;
    }
  }

  if ((sprite & 0x03) == 0x00 && (bg & 0x03) == 0x00) {
    return 0x00;  // Return EXT in, which on all known console variations is all 0
  } else if ((bg & 0x03) == 0x00) {
    return sprite | 0x10;
  } else if ((sprite & 0x03) == 0x00) {
    return bg;
  } else if ((sprite & 0x10) == 0x00) {
    return sprite | 0x10;
  } else {
    return bg;
  }
}

void PPU::sprite_evaluation() {
  // Initialize secondary OAM with 0xFF
  std::fill(std::begin(this->secondary_OAM), std::end(this->secondary_OAM), 0xFF);

  // Find the first 8 sprites that appear on the scanline and copy them to secondary OAM
  size_t n = 0;
  size_t m = 0;
  size_t sprite_num = 0;
  while (n < 64) {
    uint8_t y = this->OAM[m + n*4];
    this->secondary_OAM[sprite_num * 4] = y;

    if (this->scanline >= y && this->scanline < y + 8 + 8*((this->PPUCTRL & 0x20) != 0)) {
      if (sprite_num >= 8) {
        this->PPUSTATUS |= 0b00100000;  // Sprite overflow flag
        break;
      }

      this->secondary_OAM[sprite_num * 4 + 1] = this->OAM[m + n*4 + 1];
      this->secondary_OAM[sprite_num * 4 + 2] = this->OAM[m + n*4 + 2];
      this->secondary_OAM[sprite_num * 4 + 3] = this->OAM[m + n*4 + 3];

      sprite_num += 1;
    } else if (sprite_num >= 8) {
      m = (m + 1) % 4;
    }

    n += 1;
  }

  // Initialize the sprite units with the sprites in secondary OAM
  for (size_t i = 0; i < 8; i++) {
    uint8_t y = this->secondary_OAM[i * 4];
    uint8_t tile = this->secondary_OAM[i * 4 + 1];
    uint8_t attr = this->secondary_OAM[i * 4 + 2];
    uint8_t x = this->secondary_OAM[i * 4 + 3];

    // if (attr == 0xFF) {
    //   this->sprite_units[i] = SpriteUnit(0xFF, 0xFF, 0xFF, 0xFF);
    //   continue;
    // }

    uint8_t y_offset = this->scanline - y;
    if ((attr & 0x80) != 0) {  // Vertical flip
      y_offset = 8 + 8 * ((this->PPUCTRL & 0x20) != 0) - y_offset - 1;
    }

    uint16_t upper_addr = 0;
    uint16_t lower_addr = 0;

    if ((this->PPUCTRL & 0x20) == 0) {  // 8x8 sprites
      // Left or right pattern table
      upper_addr |= (((this->PPUCTRL & 0x08) != 0) * 0x2000);
      lower_addr |= (((this->PPUCTRL & 0x08) != 0) * 0x2000);

      // Tile index
      upper_addr |= (tile << 4);
      lower_addr |= (tile << 4);

      // Y offset
      upper_addr |= (y_offset & 0x07);
      lower_addr |= (y_offset & 0x07);

      // Bit plane
      upper_addr |= 0x08;

    } else {  // 8x16 sprites
      // Left or right pattern table
      upper_addr |= (tile & 0x01);
      lower_addr |= (tile & 0x01);

      // Tile index
      upper_addr |= ((tile & 0xFE) << 4);
      lower_addr |= ((tile & 0xFE) << 4);

      // Y offset
      upper_addr |= ((y_offset & 0x07) | ((y_offset >= 8) * 0x10));
      lower_addr |= ((y_offset & 0x07) | ((y_offset >= 8) * 0x10));

      // Bit plane
      upper_addr |= 0x08;
    }

    uint8_t upper = this->bus->read(upper_addr);
    uint8_t lower = this->bus->read(lower_addr);

    if ((attr & 0x40) != 0) {  // Horizontal flip
      upper = (upper >> 4) | (upper << 4);
      upper = ((upper & 0xCC) >> 2) | ((upper & 0x33) << 2);
      upper = ((upper & 0xAA) >> 1) | ((upper & 0x55) << 1);

      lower = (lower >> 4) | (lower << 4);
      lower = ((lower & 0xCC) >> 2) | ((lower & 0x33) << 2);
      lower = ((lower & 0xAA) >> 1) | ((lower & 0x55) << 1);
    }

    this->sprite_units[i] = SpriteUnit(upper, lower, attr, x);
  }
}
