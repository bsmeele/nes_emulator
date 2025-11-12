#include <catch2/catch_test_macros.hpp>

#include "nes.h"
#include "ppu/ppu.h"
#include "bus.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

TEST_CASE( "Sprite Unit", "[PPU]" ) {
  SpriteUnit sprite_unit = SpriteUnit(0b10010110, 0b01011010, 0b00100010, 0x04);

  REQUIRE( (sprite_unit.tick() & 0x03) == 0 );
  REQUIRE( (sprite_unit.tick() & 0x03) == 0 );
  REQUIRE( (sprite_unit.tick() & 0x03) == 0 );
  REQUIRE( (sprite_unit.tick() & 0x03) == 0 );
  REQUIRE( sprite_unit.tick() == 0b00011010 );
  REQUIRE( sprite_unit.tick() == 0b00011001 );
  REQUIRE( sprite_unit.tick() == 0b00011000 );
  REQUIRE( sprite_unit.tick() == 0b00011011 );
  REQUIRE( sprite_unit.tick() == 0b00011001 );
  REQUIRE( sprite_unit.tick() == 0b00011010 );
  REQUIRE( sprite_unit.tick() == 0b00011011 );
  REQUIRE( sprite_unit.tick() == 0b00011000 );
  REQUIRE( (sprite_unit.tick() & 0x03) == 0 );
  REQUIRE( (sprite_unit.tick() & 0x03) == 0 );
  REQUIRE( (sprite_unit.tick() & 0x03) == 0 );
  REQUIRE( (sprite_unit.tick() & 0x03) == 0 );
}

TEST_CASE( "Pixel Generation", "[PPU]" ) {
  Bus bus = Bus(1);
  PPU ppu = PPU(&bus);

  ppu.upper_shift = 0b0010010000111110;
  ppu.lower_shift = 0b0011101001011011;
  ppu.attr_shift_1 = 0b01000111;
  ppu.attr_shift_2 = 0b11110110;
  ppu.attr_1 = false;
  ppu.attr_2 = true;

  SECTION( "Background, fine X = 0" ) {
    ppu.x = 0x00;

    REQUIRE( ppu.gen_pixel() == 0b00101 );
    REQUIRE( ppu.gen_pixel() == 0b01111 );
    REQUIRE( ppu.gen_pixel() == 0b01110 );

    REQUIRE( ppu.upper_shift == 0b0000010010000111 );
    REQUIRE( ppu.lower_shift == 0b1110011101001011 );
    REQUIRE( ppu.attr_shift_1 == 0b00001000 );
    REQUIRE( ppu.attr_shift_2 == 0b11111110 );
  }

  SECTION( "Backgruond, fine X = 4" ) {
    ppu.x = 0x04;

    REQUIRE( ppu.gen_pixel() == 0b01011 );
    REQUIRE( ppu.gen_pixel() == 0b01010 );
    REQUIRE( ppu.gen_pixel() == 0b01101 );

    REQUIRE( ppu.upper_shift == 0b0000010010000111 );
    REQUIRE( ppu.lower_shift == 0b1110011101001011 );
    REQUIRE( ppu.attr_shift_1 == 0b00001000 );
    REQUIRE( ppu.attr_shift_2 == 0b11111110 );
  }

  SECTION( "Sprites" ) {
    ppu.sprite_units[0] = SpriteUnit(0b01000010, 0b10011100, 0b01000000, 0x02);
    ppu.sprite_units[1] = SpriteUnit(0b10110100, 0b11011110, 0b01000011, 0x00);

    REQUIRE( ppu.gen_pixel() == 0b11111 );
    REQUIRE( ppu.gen_pixel() == 0b11101 );
    REQUIRE( ppu.gen_pixel() == 0b10001 );
    REQUIRE( ppu.gen_pixel() == 0b10010 );
    REQUIRE( ppu.gen_pixel() == 0b11101 );

    REQUIRE( ppu.sprite_units[0].upper == 0b00010000 );
    REQUIRE( ppu.sprite_units[0].lower == 0b11100000 );
    REQUIRE( ppu.sprite_units[0].counter == 0x00 );
    REQUIRE( ppu.sprite_units[1].upper == 0b10000000 );
    REQUIRE( ppu.sprite_units[1].lower == 0b11000000 );
    REQUIRE( ppu.sprite_units[1].counter == 0x00 );
  }

  SECTION( "Sprite/background selection" ) {
    ppu.sprite_units[0] = SpriteUnit(0b00010000, 0b01011110, 0b01100011, 0x00);

    ppu.upper_shift = 0b0010010000111100;
    ppu.lower_shift = 0b0011101001011000;

    REQUIRE( ppu.gen_pixel() == 0b00000 );
    REQUIRE( ppu.gen_pixel() == 0b11101 );

    ppu.sprite_units[0].priority = false;

    REQUIRE( ppu.gen_pixel() == 0b01110 );
    REQUIRE( ppu.gen_pixel() == 0b11111 );

    ppu.sprite_units[0].priority = true;

    REQUIRE( ppu.gen_pixel() == 0b01011 );
  }
}

TEST_CASE( "Sprite evaluation", "[PPU]" ) {
  NES nes = NES();

  std::string filename = "../test_roms/sprite_test.nes";
  std::ifstream file(filename, std::ios::binary | std::ios::ate);

  if (!file) {
    throw std::runtime_error("Could not open file: " + filename);
  }

  std::streamsize filesize = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> rom(filesize);

  if (!file.read(reinterpret_cast<char*>(rom.data()), filesize)) {
    throw std::runtime_error("Could not read file: " + filename);
  }

  nes.load_rom(rom);

  nes.ppu.PPUCTRL &= ~0b00101000;

  // Fill OAM
  uint8_t tmp_oam[] = {
    0x00, 0x00, 0b00000000, 0x00,  // 0 priority, no flip
    0x01, 0x01, 0b00100001, 0x08,  // 1 priority, no flip
    0x02, 0x02, 0b01000010, 0x10,  // 0 priority, horizontal flip
    0x03, 0x03, 0b01100011, 0x18,  // 1 priority, horizontal flip
    0x04, 0x04, 0b10000000, 0x20,  // 0 priority, vertical flip
    0x05, 0x05, 0b10100001, 0x28,  // 1 priority, vertical flip
    0x06, 0x06, 0b11000010, 0x30,  // 0 priority, vertical and horizontal flip
    0x07, 0x07, 0b11100011, 0x38,  // 1 priority, vertical and horizontal flip
    0x08, 0x08, 0b00000000, 0x00,  // 0 priority, no flip
    0x08, 0x09, 0b00100001, 0x40,  // 1 priority, no flip
  };
  std::copy(std::begin(tmp_oam), std::end(tmp_oam), nes.ppu.OAM);

  SECTION( "Less than 8 sprites, 8x8 sprite size" ) {
    nes.ppu.scanline = 6;
    nes.ppu.sprite_evaluation();

    REQUIRE( nes.ppu.sprite_units[0].upper == 0x35 );
    REQUIRE( nes.ppu.sprite_units[0].lower == 0x33 );
    REQUIRE( nes.ppu.sprite_units[0].palette == 0b00 );
    REQUIRE( nes.ppu.sprite_units[0].priority == false );
    REQUIRE( nes.ppu.sprite_units[0].counter == 0x00 );

    REQUIRE( nes.ppu.sprite_units[1].upper == 0x09 );
    REQUIRE( nes.ppu.sprite_units[1].lower == 0x34 );
    REQUIRE( nes.ppu.sprite_units[1].palette == 0b01 );
    REQUIRE( nes.ppu.sprite_units[1].priority == true );
    REQUIRE( nes.ppu.sprite_units[1].counter == 0x08 );

    REQUIRE( nes.ppu.sprite_units[2].upper == 0x90 );
    REQUIRE( nes.ppu.sprite_units[2].lower == 0xCC );
    REQUIRE( nes.ppu.sprite_units[2].palette == 0b10 );
    REQUIRE( nes.ppu.sprite_units[2].priority == false );
    REQUIRE( nes.ppu.sprite_units[2].counter == 0x10 );

    REQUIRE( nes.ppu.sprite_units[3].upper == 0x8C );
    REQUIRE( nes.ppu.sprite_units[3].lower == 0x8C );
    REQUIRE( nes.ppu.sprite_units[3].palette == 0b11 );
    REQUIRE( nes.ppu.sprite_units[3].priority == true );
    REQUIRE( nes.ppu.sprite_units[3].counter == 0x18 );

    REQUIRE( nes.ppu.sprite_units[4].upper == 0x31 );
    REQUIRE( nes.ppu.sprite_units[4].lower == 0x09 );
    REQUIRE( nes.ppu.sprite_units[4].palette == 0b00 );
    REQUIRE( nes.ppu.sprite_units[4].priority == false );
    REQUIRE( nes.ppu.sprite_units[4].counter == 0x20 );

    REQUIRE( nes.ppu.sprite_units[5].upper == 0x36 );
    REQUIRE( nes.ppu.sprite_units[5].lower == 0x32 );
    REQUIRE( nes.ppu.sprite_units[5].palette == 0b01 );
    REQUIRE( nes.ppu.sprite_units[5].priority == true );
    REQUIRE( nes.ppu.sprite_units[5].counter == 0x28 );

    REQUIRE( nes.ppu.sprite_units[6].upper == 0x50 );
    REQUIRE( nes.ppu.sprite_units[6].lower == 0x90 );
    REQUIRE( nes.ppu.sprite_units[6].palette == 0b10 );
    REQUIRE( nes.ppu.sprite_units[6].priority == false );
    REQUIRE( nes.ppu.sprite_units[6].counter == 0x30 );

    // Upper and lower byte of empty sprite units are filled with garbage
    REQUIRE( nes.ppu.sprite_units[7].palette == 0b11 );
    REQUIRE( nes.ppu.sprite_units[7].priority == true );
    REQUIRE( nes.ppu.sprite_units[7].counter == 0xFF );

    REQUIRE( (nes.ppu.PPUSTATUS & 0b00100000) == 0 );
  }

  SECTION( "Exactly 8 sprites, 8x8 sprite size") {
    nes.ppu.scanline = 7;
    nes.ppu.sprite_evaluation();

    REQUIRE( nes.ppu.sprite_units[0].upper == 0x0A );
    REQUIRE( nes.ppu.sprite_units[0].lower == 0x09 );
    REQUIRE( nes.ppu.sprite_units[0].palette == 0b00 );
    REQUIRE( nes.ppu.sprite_units[0].priority == false );
    REQUIRE( nes.ppu.sprite_units[0].counter == 0x00 );

    REQUIRE( nes.ppu.sprite_units[1].upper == 0x31 );
    REQUIRE( nes.ppu.sprite_units[1].lower == 0x09 );
    REQUIRE( nes.ppu.sprite_units[1].palette == 0b01 );
    REQUIRE( nes.ppu.sprite_units[1].priority == true );
    REQUIRE( nes.ppu.sprite_units[1].counter == 0x08 );

    REQUIRE( nes.ppu.sprite_units[2].upper == 0x8C );
    REQUIRE( nes.ppu.sprite_units[2].lower == 0x90 );
    REQUIRE( nes.ppu.sprite_units[2].palette == 0b10 );
    REQUIRE( nes.ppu.sprite_units[2].priority == false );
    REQUIRE( nes.ppu.sprite_units[2].counter == 0x10 );

    REQUIRE( nes.ppu.sprite_units[3].upper == 0xEC );
    REQUIRE( nes.ppu.sprite_units[3].lower == 0x50 );
    REQUIRE( nes.ppu.sprite_units[3].palette == 0b11 );
    REQUIRE( nes.ppu.sprite_units[3].priority == true );
    REQUIRE( nes.ppu.sprite_units[3].counter == 0x18 );

    REQUIRE( nes.ppu.sprite_units[4].upper == 0x09 );
    REQUIRE( nes.ppu.sprite_units[4].lower == 0x38 );
    REQUIRE( nes.ppu.sprite_units[4].palette == 0b00 );
    REQUIRE( nes.ppu.sprite_units[4].priority == false );
    REQUIRE( nes.ppu.sprite_units[4].counter == 0x20 );

    REQUIRE( nes.ppu.sprite_units[5].upper == 0x39 );
    REQUIRE( nes.ppu.sprite_units[5].lower == 0x32 );
    REQUIRE( nes.ppu.sprite_units[5].palette == 0b01 );
    REQUIRE( nes.ppu.sprite_units[5].priority == true );
    REQUIRE( nes.ppu.sprite_units[5].counter == 0x28 );

    REQUIRE( nes.ppu.sprite_units[6].upper == 0xEC );
    REQUIRE( nes.ppu.sprite_units[6].lower == 0x1C );
    REQUIRE( nes.ppu.sprite_units[6].palette == 0b10 );
    REQUIRE( nes.ppu.sprite_units[6].priority == false );
    REQUIRE( nes.ppu.sprite_units[6].counter == 0x30 );

    REQUIRE( nes.ppu.sprite_units[7].upper == 0xAC );
    REQUIRE( nes.ppu.sprite_units[7].lower == 0x1C );
    REQUIRE( nes.ppu.sprite_units[7].palette == 0b11 );
    REQUIRE( nes.ppu.sprite_units[7].priority == true );
    REQUIRE( nes.ppu.sprite_units[7].counter == 0x38 );

    REQUIRE( (nes.ppu.PPUSTATUS & 0b00100000) == 0 );
  }

  SECTION( "More than 8 sprites, 8x8 sprite size") {
    nes.ppu.scanline = 8;
    nes.ppu.sprite_evaluation();

    REQUIRE( nes.ppu.sprite_units[0].upper == 0x31 );
    REQUIRE( nes.ppu.sprite_units[0].lower == 0x31 );
    REQUIRE( nes.ppu.sprite_units[0].palette == 0b01 );
    REQUIRE( nes.ppu.sprite_units[0].priority == true );
    REQUIRE( nes.ppu.sprite_units[0].counter == 0x08 );

    REQUIRE( nes.ppu.sprite_units[1].upper == 0x9C );
    REQUIRE( nes.ppu.sprite_units[1].lower == 0x4C );
    REQUIRE( nes.ppu.sprite_units[1].palette == 0b10 );
    REQUIRE( nes.ppu.sprite_units[1].priority == false );
    REQUIRE( nes.ppu.sprite_units[1].counter == 0x10 );

    REQUIRE( nes.ppu.sprite_units[2].upper == 0xAC );
    REQUIRE( nes.ppu.sprite_units[2].lower == 0x8C );
    REQUIRE( nes.ppu.sprite_units[2].palette == 0b11 );
    REQUIRE( nes.ppu.sprite_units[2].priority == true );
    REQUIRE( nes.ppu.sprite_units[2].counter == 0x18 );

    REQUIRE( nes.ppu.sprite_units[3].upper == 0x34 );
    REQUIRE( nes.ppu.sprite_units[3].lower == 0x32 );
    REQUIRE( nes.ppu.sprite_units[3].palette == 0b00 );
    REQUIRE( nes.ppu.sprite_units[3].priority == false );
    REQUIRE( nes.ppu.sprite_units[3].counter == 0x20 );

    REQUIRE( nes.ppu.sprite_units[4].upper == 0x31 );
    REQUIRE( nes.ppu.sprite_units[4].lower == 0x09 );
    REQUIRE( nes.ppu.sprite_units[4].palette == 0b01 );
    REQUIRE( nes.ppu.sprite_units[4].priority == true );
    REQUIRE( nes.ppu.sprite_units[4].counter == 0x28 );

    REQUIRE( nes.ppu.sprite_units[5].upper == 0x6C );
    REQUIRE( nes.ppu.sprite_units[5].lower == 0x2C );
    REQUIRE( nes.ppu.sprite_units[5].palette == 0b10 );
    REQUIRE( nes.ppu.sprite_units[5].priority == false );
    REQUIRE( nes.ppu.sprite_units[5].counter == 0x30 );

    REQUIRE( nes.ppu.sprite_units[6].upper == 0x8C );
    REQUIRE( nes.ppu.sprite_units[6].lower == 0xCC );
    REQUIRE( nes.ppu.sprite_units[6].palette == 0b11 );
    REQUIRE( nes.ppu.sprite_units[6].priority == true );
    REQUIRE( nes.ppu.sprite_units[6].counter == 0x38 );

    REQUIRE( nes.ppu.sprite_units[7].upper == 0x31 );
    REQUIRE( nes.ppu.sprite_units[7].lower == 0x09 );
    REQUIRE( nes.ppu.sprite_units[7].palette == 0b00 );
    REQUIRE( nes.ppu.sprite_units[7].priority == false );
    REQUIRE( nes.ppu.sprite_units[7].counter == 0x00 );

    REQUIRE( (nes.ppu.PPUSTATUS & 0b00100000) != 0 );
  }

  // Test 8x16
  SECTION( "8x16 sprites" ) {
    uint8_t tmp_oam[] = {
      0x00, 0x00, 0b00000000, 0x00,  // 0 priority, no flip
      0x01, 0x02, 0b00100001, 0x08,  // 1 priority, no flip
      0x02, 0x04, 0b01000010, 0x10,  // 0 priority, horizontal flip
      0x03, 0x06, 0b01100011, 0x18,  // 1 priority, horizontal flip
      0x04, 0x08, 0b10000000, 0x20,  // 0 priority, vertical flip
      0x05, 0x0A, 0b10100001, 0x28,  // 1 priority, vertical flip
      0x06, 0x0C, 0b11000010, 0x30,  // 0 priority, vertical and horizontal flip
      0x07, 0x0E, 0b11100011, 0x38,  // 1 priority, vertical and horizontal flip
    };
    std::copy(std::begin(tmp_oam), std::end(tmp_oam), nes.ppu.OAM);

    nes.ppu.PPUCTRL |= 0b00100000;
    nes.ppu.scanline = 7;
    nes.ppu.sprite_evaluation();

    REQUIRE( nes.ppu.sprite_units[0].upper == 0x0A );
    REQUIRE( nes.ppu.sprite_units[0].lower == 0x09 );
    REQUIRE( nes.ppu.sprite_units[0].palette == 0b00 );
    REQUIRE( nes.ppu.sprite_units[0].priority == false );
    REQUIRE( nes.ppu.sprite_units[0].counter == 0x00 );

    REQUIRE( nes.ppu.sprite_units[1].upper == 0x39 );
    REQUIRE( nes.ppu.sprite_units[1].lower == 0x32 );
    REQUIRE( nes.ppu.sprite_units[1].palette == 0b01 );
    REQUIRE( nes.ppu.sprite_units[1].priority == true );
    REQUIRE( nes.ppu.sprite_units[1].counter == 0x08 );

    REQUIRE( nes.ppu.sprite_units[2].upper == 0x8C );
    REQUIRE( nes.ppu.sprite_units[2].lower == 0x90 );
    REQUIRE( nes.ppu.sprite_units[2].palette == 0b10 );
    REQUIRE( nes.ppu.sprite_units[2].priority == false );
    REQUIRE( nes.ppu.sprite_units[2].counter == 0x10 );

    REQUIRE( nes.ppu.sprite_units[3].upper == 0x8C );
    REQUIRE( nes.ppu.sprite_units[3].lower == 0x8C );
    REQUIRE( nes.ppu.sprite_units[3].palette == 0b11 );
    REQUIRE( nes.ppu.sprite_units[3].priority == true );
    REQUIRE( nes.ppu.sprite_units[3].counter == 0x18 );

    REQUIRE( nes.ppu.sprite_units[4].upper == 0x34 );
    REQUIRE( nes.ppu.sprite_units[4].lower == 0x31 );
    REQUIRE( nes.ppu.sprite_units[4].palette == 0b00 );
    REQUIRE( nes.ppu.sprite_units[4].priority == false );
    REQUIRE( nes.ppu.sprite_units[4].counter == 0x20 );

    REQUIRE( nes.ppu.sprite_units[5].upper == 0x36 );
    REQUIRE( nes.ppu.sprite_units[5].lower == 0x31 );
    REQUIRE( nes.ppu.sprite_units[5].palette == 0b01 );
    REQUIRE( nes.ppu.sprite_units[5].priority == true );
    REQUIRE( nes.ppu.sprite_units[5].counter == 0x28 );

    REQUIRE( nes.ppu.sprite_units[6].upper == 0x2C );
    REQUIRE( nes.ppu.sprite_units[6].lower == 0x2C );
    REQUIRE( nes.ppu.sprite_units[6].palette == 0b10 );
    REQUIRE( nes.ppu.sprite_units[6].priority == false );
    REQUIRE( nes.ppu.sprite_units[6].counter == 0x30 );

    REQUIRE( nes.ppu.sprite_units[7].upper == 0x90 );
    REQUIRE( nes.ppu.sprite_units[7].lower == 0x90 );
    REQUIRE( nes.ppu.sprite_units[7].palette == 0b11 );
    REQUIRE( nes.ppu.sprite_units[7].priority == true );
    REQUIRE( nes.ppu.sprite_units[7].counter == 0x38 );

    REQUIRE( (nes.ppu.PPUSTATUS & 0b00100000) == 0 );
  }
}
