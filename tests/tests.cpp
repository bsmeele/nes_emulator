#include <catch2/catch_test_macros.hpp>

#include "nes.h"

TEST_CASE( "RAM read/write", "[RAM]" ) {
  NES nes = NES();
  nes.cpu_bus.write(0x0001, 1);
  nes.cpu_bus.write(0x0802, 2);
  nes.cpu_bus.write(0x1003, 3);
  nes.cpu_bus.write(0x1804, 5);

  REQUIRE(  nes.cpu_bus.read(0x0001) == 1 );
  REQUIRE(  nes.cpu_bus.read(0x0002) == 2 );
  REQUIRE(  nes.cpu_bus.read(0x0003) == 3 );
  REQUIRE(  nes.cpu_bus.read(0x0004) == 5 );
  
  REQUIRE(  nes.cpu_bus.read(0x0801) == 1 );
  REQUIRE(  nes.cpu_bus.read(0x0802) == 2 );
  REQUIRE(  nes.cpu_bus.read(0x0803) == 3 );
  REQUIRE(  nes.cpu_bus.read(0x0804) == 5 );

  REQUIRE(  nes.cpu_bus.read(0x1001) == 1 );
  REQUIRE(  nes.cpu_bus.read(0x1002) == 2 );
  REQUIRE(  nes.cpu_bus.read(0x1003) == 3 );
  REQUIRE(  nes.cpu_bus.read(0x1004) == 5 );

  REQUIRE(  nes.cpu_bus.read(0x1801) == 1 );
  REQUIRE(  nes.cpu_bus.read(0x1802) == 2 );
  REQUIRE(  nes.cpu_bus.read(0x1803) == 3 );
  REQUIRE(  nes.cpu_bus.read(0x1804) == 5 );
}
