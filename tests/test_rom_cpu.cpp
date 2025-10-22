#include <catch2/catch_test_macros.hpp>

#include "nes.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

TEST_CASE( "Test ROM CPU", "[CPU]" ) {
  NES nes = NES();

  SECTION( "Fibonacci" ) {
    std::string filename = "../test_roms/fibonacci.nes";
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

    while (nes.cpu.program_counter != 0xFFFE) {
      nes.cpu.tick();
    }

    int a = 0;
    int b = 1;
    uint16_t address = 0x0000;
    do {
      REQUIRE( nes.bus.read(address) == a );

      a += b;
      std::swap(a, b);

      address += 1;
    } while (nes.bus.read(address) != 0x00);
  }

  SECTION( "Arithmetic" ) {
    std::string filename = "../test_roms/arithmetic.nes";
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

    while (nes.cpu.program_counter != 0xFFFE) {
      nes.cpu.tick();
    }
    
    REQUIRE( nes.bus.read(0x0002) == 0x00 );  // 0 * 35
    REQUIRE( nes.bus.read(0x0005) == 0x00 );  // 6 * 0
    REQUIRE( nes.bus.read(0x0008) == 0xD2 );  // 6 * 35
    REQUIRE( nes.bus.read(0x000B) == 0x00 );  // 0 / 8
    REQUIRE( nes.bus.read(0x000C) == 0x00 );  // 0 % 8
    REQUIRE( nes.bus.read(0x000F) == 0xFF );  // 210 / 0
    REQUIRE( nes.bus.read(0x0010) == 0x00 );  // 210 % 0
    REQUIRE( nes.bus.read(0x0013) == 0x1A );  // 210 / 8
    REQUIRE( nes.bus.read(0x0014) == 0x02 );  // 210 % 8
    REQUIRE( nes.bus.read(0x0017) == 0x1E );  // 210 / 7
    REQUIRE( nes.bus.read(0x0018) == 0x00 );  // 210 % 7
  }

  SECTION( "Sorting" ) {
    std::string filename = "../test_roms/sorting.nes";
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

    while (nes.cpu.program_counter != 0xFFFE) {
      nes.cpu.tick();
    }
    
    // Bubble sort
    REQUIRE( nes.bus.read(0x0000) == 0x02 );
    REQUIRE( nes.bus.read(0x0001) == 0x22 );
    REQUIRE( nes.bus.read(0x0002) == 0x2C );
    REQUIRE( nes.bus.read(0x0003) == 0x2F );
    REQUIRE( nes.bus.read(0x0004) == 0x3F );
    REQUIRE( nes.bus.read(0x0005) == 0x47 );
    REQUIRE( nes.bus.read(0x0006) == 0x5D );
    REQUIRE( nes.bus.read(0x0007) == 0x66 );
    REQUIRE( nes.bus.read(0x0008) == 0x89 );
    REQUIRE( nes.bus.read(0x0009) == 0x9B );
    REQUIRE( nes.bus.read(0x000A) == 0xBC );
    REQUIRE( nes.bus.read(0x000B) == 0xD1 );
    REQUIRE( nes.bus.read(0x000C) == 0xD8 );
    REQUIRE( nes.bus.read(0x000D) == 0xE3 );
    REQUIRE( nes.bus.read(0x000E) == 0xEA );
    REQUIRE( nes.bus.read(0x000F) == 0xF4 );

    // Insertion sort
    REQUIRE( nes.bus.read(0x0010) == 0x02 );
    REQUIRE( nes.bus.read(0x0011) == 0x22 );
    REQUIRE( nes.bus.read(0x0012) == 0x2C );
    REQUIRE( nes.bus.read(0x0013) == 0x2F );
    REQUIRE( nes.bus.read(0x0014) == 0x3F );
    REQUIRE( nes.bus.read(0x0015) == 0x47 );
    REQUIRE( nes.bus.read(0x0016) == 0x5D );
    REQUIRE( nes.bus.read(0x0017) == 0x66 );
    REQUIRE( nes.bus.read(0x0018) == 0x89 );
    REQUIRE( nes.bus.read(0x0019) == 0x9B );
    REQUIRE( nes.bus.read(0x001A) == 0xBC );
    REQUIRE( nes.bus.read(0x001B) == 0xD1 );
    REQUIRE( nes.bus.read(0x001C) == 0xD8 );
    REQUIRE( nes.bus.read(0x001D) == 0xE3 );
    REQUIRE( nes.bus.read(0x001E) == 0xEA );
    REQUIRE( nes.bus.read(0x001F) == 0xF4 );

    // // Quicksort    
    REQUIRE( nes.bus.read(0x0020) == 0x02 );
    REQUIRE( nes.bus.read(0x0021) == 0x22 );
    REQUIRE( nes.bus.read(0x0022) == 0x2C );
    REQUIRE( nes.bus.read(0x0023) == 0x2F );
    REQUIRE( nes.bus.read(0x0024) == 0x3F );
    REQUIRE( nes.bus.read(0x0025) == 0x47 );
    REQUIRE( nes.bus.read(0x0026) == 0x5D );
    REQUIRE( nes.bus.read(0x0027) == 0x66 );
    REQUIRE( nes.bus.read(0x0028) == 0x89 );
    REQUIRE( nes.bus.read(0x0029) == 0x9B );
    REQUIRE( nes.bus.read(0x002A) == 0xBC );
    REQUIRE( nes.bus.read(0x002B) == 0xD1 );
    REQUIRE( nes.bus.read(0x002C) == 0xD8 );
    REQUIRE( nes.bus.read(0x002D) == 0xE3 );
    REQUIRE( nes.bus.read(0x002E) == 0xEA );
    REQUIRE( nes.bus.read(0x002F) == 0xF4 );
  }
}
