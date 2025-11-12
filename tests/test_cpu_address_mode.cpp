#include <catch2/catch_test_macros.hpp>

#include "nes.h"

TEST_CASE( "CPU address modes", "[CPU]" ) {
  NES nes = NES();

  nes.cpu_bus.write(0x0001, 0x34);
  nes.cpu_bus.write(0x0002, 0x12);

  nes.cpu.program_counter = 0x0001;

  SECTION( "absolute" ) {

    auto address = nes.cpu.addressing_absolute();

    REQUIRE( address.value() == 0x1234 );
    REQUIRE( nes.cpu.lockout_counter == 2);
    REQUIRE( nes.cpu.program_counter == 0x0003 );
  }

  SECTION( "absoluteX" ) {
    SECTION( "No page crossing" ) {
      nes.cpu.x_index = 0x65;

      auto address = nes.cpu.addressing_absoluteX();

      REQUIRE( address.value() == 0x1299 );
      REQUIRE( nes.cpu.lockout_counter == 2);
      REQUIRE( nes.cpu.program_counter == 0x0003 );
    }

    SECTION( "Page crossing" ) {
      nes.cpu.x_index = 0xCD;

      auto address = nes.cpu.addressing_absoluteX();

      REQUIRE( address.value() == 0x1301 );
      REQUIRE( nes.cpu.lockout_counter == 3 );
      REQUIRE( nes.cpu.program_counter == 0x0003 );
    }
  }

  SECTION( "absoluteY" ) {
    SECTION( "No page crossing" ) {
      nes.cpu.y_index = 0x65;

      auto address = nes.cpu.addressing_absoluteY();

      REQUIRE( address.value() == 0x1299 );
      REQUIRE( nes.cpu.lockout_counter == 2);
      REQUIRE( nes.cpu.program_counter == 0x0003 );
    }

    SECTION( "Page crossing" ) {
      nes.cpu.y_index = 0xCD;

      auto address = nes.cpu.addressing_absoluteY();

      REQUIRE( address.value() == 0x1301 );
      REQUIRE( nes.cpu.lockout_counter == 3 );
      REQUIRE( nes.cpu.program_counter == 0x0003 );
    }
  }

  SECTION( "Accumulator" ) {
    auto address = nes.cpu.addressing_accumulator();

    REQUIRE( address.has_value() == false );
    REQUIRE( nes.cpu.lockout_counter == 0 );
    REQUIRE( nes.cpu.program_counter == 0x0001 );
  }

  SECTION( "Immediate" ) {
    auto address = nes.cpu.addressing_immediate();

    REQUIRE( address.value() == 0x0001 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
    REQUIRE( nes.cpu.program_counter == 0x0002 );
  }

  SECTION( "Implied" ) {
    auto address = nes.cpu.addressing_implied();

    REQUIRE( address.has_value() == false );
    REQUIRE( nes.cpu.lockout_counter == 0 );
    REQUIRE( nes.cpu.program_counter == 0x0001 );
  }

  SECTION( "Indirect" ) {
    nes.cpu_bus.write(0x1234, 0x23);
    nes.cpu_bus.write(0x1235, 0x01);

    auto address = nes.cpu.addressing_indirect();

    REQUIRE( address.value() == 0x0123 );
    REQUIRE( nes.cpu.lockout_counter == 4 );
    REQUIRE( nes.cpu.program_counter == 0x0003 );
  }

  SECTION( "IndirectX ") {
    SECTION( "No page crossing" ) {
      nes.cpu.x_index = 0x65;

      nes.cpu_bus.write(0x0099, 0x23);
      nes.cpu_bus.write(0x009A, 0x01);

      auto address = nes.cpu.addressing_indirectX();

      REQUIRE( address.value() == 0x0123 );
      REQUIRE( nes.cpu.lockout_counter == 4);
      REQUIRE( nes.cpu.program_counter == 0x0002 );
    }

    SECTION( "No page crossing (overflow)" ) {
      nes.cpu.x_index = 0xDE;

      nes.cpu_bus.write(0x0012, 0x23);
      nes.cpu_bus.write(0x0013, 0x01);

      auto address = nes.cpu.addressing_indirectX();

      REQUIRE( address.value() == 0x0123 );
      REQUIRE( nes.cpu.lockout_counter == 4 );
      REQUIRE( nes.cpu.program_counter == 0x0002 );
    }
  }

  SECTION( "IndirectY ") {
    SECTION( "No page crossing" ) {
      nes.cpu.y_index = 0x65;

      nes.cpu_bus.write(0x0034, 0x23);
      nes.cpu_bus.write(0x0035, 0x01);

      auto address = nes.cpu.addressing_indirectY();

      REQUIRE( address.value() == 0x0188 );
      REQUIRE( nes.cpu.lockout_counter == 3);
      REQUIRE( nes.cpu.program_counter == 0x0002 );
    }

    SECTION( "Page crossing" ) {
      nes.cpu.y_index = 0xEF;

      nes.cpu_bus.write(0x0034, 0x23);
      nes.cpu_bus.write(0x0035, 0x01);

      auto address = nes.cpu.addressing_indirectY();

      REQUIRE( address.value() == 0x0212 );
      REQUIRE( nes.cpu.lockout_counter == 4 );
      REQUIRE( nes.cpu.program_counter == 0x0002 );
    }
  }

  SECTION( "Relative" ) {
    auto address = nes.cpu.addressing_immediate();

    REQUIRE( address.value() == 0x0001 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
    REQUIRE( nes.cpu.program_counter == 0x0002 );
  }

  SECTION( "Zeropage" ) {
    auto address = nes.cpu.addressing_zeropage();

    REQUIRE( address.value() == 0x0034 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
    REQUIRE( nes.cpu.program_counter == 0x0002 );
  }

  SECTION( "ZeropageX" ) {
    SECTION( "No page crossing" ) {
      nes.cpu.x_index = 0x65;

      auto address = nes.cpu.addressing_zeropageX();

      REQUIRE( address.value() == 0x0099 );
      REQUIRE( nes.cpu.lockout_counter == 2);
      REQUIRE( nes.cpu.program_counter == 0x0002 );
    }

    SECTION( "No page crossing (overflow)" ) {
      nes.cpu.x_index = 0xCD;

      auto address = nes.cpu.addressing_zeropageX();

      REQUIRE( address.value() == 0x0001 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
      REQUIRE( nes.cpu.program_counter == 0x0002 );
    }
  }

  SECTION( "ZeropageY" ) {
    SECTION( "No page crossing" ) {
      nes.cpu.y_index = 0x65;

      auto address = nes.cpu.addressing_zeropageY();

      REQUIRE( address.value() == 0x0099 );
      REQUIRE( nes.cpu.lockout_counter == 2);
      REQUIRE( nes.cpu.program_counter == 0x0002 );
    }

    SECTION( "No page crossing (overflow)" ) {
      nes.cpu.y_index = 0xCD;

      auto address = nes.cpu.addressing_zeropageY();

      REQUIRE( address.value() == 0x0001 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
      REQUIRE( nes.cpu.program_counter == 0x0002 );
    }
  }
}
