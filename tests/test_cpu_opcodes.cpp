#include <catch2/catch_test_macros.hpp>

#include "nes.h"


TEST_CASE( "CPU opcodes", "[CPU]" ) {
  NES nes = NES();

  SECTION( "0x00 (BRK impl)" ) {
    nes.bus.write(0x0012, 0x00);  // BRK
    nes.bus.write(0x0013, 0x38);  // SEC, should not be executed
    nes.bus.write(0x0014, 0x4C);  // JMP, should not be executed
    nes.bus.write(0x0015, 0x34);  // Lower adress bytes of JMP, should not be executed
    nes.bus.write(0x0016, 0x12);  // Upper adress byres of JMP, should not be executed

    nes.cpu.program_counter = 0x0012;
    nes.cpu.stack_pointer = 0x34;
    nes.cpu.flag = 0x4A;

    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.bus.read(0x0134) == 0x00 );
    REQUIRE( nes.bus.read(0x0133) == 0x14 );
    REQUIRE( nes.bus.read(0x0132) == 0x7A );
    REQUIRE( nes.cpu.stack_pointer == 0x31 );

    REQUIRE( nes.cpu.program_counter == 0xFFFE );
    REQUIRE( nes.cpu.flag == 0x4E );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x01 (ORA X,ind)" ) {
    nes.bus.write(0x0012, 0x01);  // ORA X,ind
    nes.bus.write(0x0013, 0x0E);  // Lower address byte
    nes.bus.write(0x0014, 0x00);  // BRK, should not be executed
    nes.bus.write(0x0010, 0x35);  // Lower address byte
    nes.bus.write(0x0011, 0x01);  // Upper address byte
    nes.bus.write(0x0135, 0x42);  // Operand

    nes.cpu.program_counter = 0x0012;
    nes.cpu.x_index = 0x02;
    nes.cpu.accumulator = 0xCC;

    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.cpu.accumulator == 0xCE );
    REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
    REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

    REQUIRE( nes.cpu.program_counter == 0x0014 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x02 (Not supported)" ) {
    nes.bus.write(0x0012, 0x02);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x03 (Not supported)" ) {
    nes.bus.write(0x0012, 0x03);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x04 (Not supported)" ) {
    nes.bus.write(0x0012, 0x04);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x05 (ORA zpg)" ) {
    nes.bus.write(0x0012, 0x05);  // ORA zpg
    nes.bus.write(0x0013, 0x0E);  // Lower address byte
    nes.bus.write(0x0014, 0x00);  // BRK, should not be executed
    nes.bus.write(0x000E, 0x42);  // Opperand

    nes.cpu.program_counter = 0x0012;
    nes.cpu.accumulator = 0xCC;

    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.cpu.accumulator == 0xCE );
    REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
    REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

    REQUIRE( nes.cpu.program_counter == 0x0014 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x06 (ASL zpg)" ) {
    nes.bus.write(0x0012, 0x06);  // ASL zpg
    nes.bus.write(0x0013, 0x0E);  // Lower address byte
    nes.bus.write(0x0014, 0x00);  // BRK, should not be executed
    nes.bus.write(0x000E, 0x42);  // Opperand

    nes.cpu.program_counter = 0x0012;

    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.bus.read(0x000E) == 0x84 );
    REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
    REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

    REQUIRE( nes.cpu.program_counter == 0x0014 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x07 (Not supported)" ) {
    nes.bus.write(0x0012, 0x07);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x08 (PHP impl)" ) {
    nes.bus.write(0x0012, 0x08);  // impl zpg
    nes.bus.write(0x0013, 0x00);  // BRK, should not be executed

    nes.cpu.program_counter = 0x0012;
    nes.cpu.stack_pointer = 0x34;
    nes.cpu.flag = 0x83;

    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.bus.read(0x0134) == 0xB3 );
    REQUIRE( nes.cpu.stack_pointer == 0x33 );

    REQUIRE( nes.cpu.program_counter == 0x0013 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x09 (ORA #)" ) {
    nes.bus.write(0x0012, 0x09);  // ORA #
    nes.bus.write(0x0013, 0x42);  // Opperand
    nes.bus.write(0x0014, 0x00);  // BRK, should not be executed

    nes.cpu.program_counter = 0x0012;
    nes.cpu.accumulator = 0xCC;

    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.cpu.accumulator == 0xCE );
    REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
    REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

    REQUIRE( nes.cpu.program_counter == 0x0014 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x0A (ASL A)" ) {
    nes.bus.write(0x0012, 0x0A);  // ASL A
    nes.bus.write(0x0013, 0x00);  // BRK, should not be executed

    nes.cpu.program_counter = 0x0012;
    nes.cpu.accumulator = 0x42;

    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.bus.read(0x000E) == 0x84 );
    REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
    REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

    REQUIRE( nes.cpu.program_counter == 0x0013 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x0B (Not supported)" ) {
    nes.bus.write(0x0012, 0x0B);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x0C (Not supported)" ) {
    nes.bus.write(0x0012, 0x0C);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x0D (ORA abs)" ) {
    nes.bus.write(0x0012, 0x0D);  // ORA abs
    nes.bus.write(0x0013, 0x35);  // Lower byte
    nes.bus.write(0x0014, 0x01);  // Upper byte
    nes.bus.write(0x0015, 0x00);  // BRK, should not be executed
    nes.bus.write(0x0135, 0x42);  // Operand

    nes.cpu.program_counter = 0x0012;
    nes.cpu.accumulator = 0xCC;

    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.cpu.accumulator == 0xCE );
    REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
    REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

    REQUIRE( nes.cpu.program_counter == 0x0015 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x0E (ASL abs)" ) {
    nes.bus.write(0x0012, 0x0E);  // ASL abs
    nes.bus.write(0x0013, 0x35);  // Lower byte
    nes.bus.write(0x0014, 0x01);  // Upper byte
    nes.bus.write(0x0015, 0x00);  // BRK, should not be executed
    nes.bus.write(0x0135, 0x42);  // Operand

    nes.cpu.program_counter = 0x0012;

    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.bus.read(0x0135) == 0x84 );
    REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
    REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

    REQUIRE( nes.cpu.program_counter == 0x0015 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x0F (Not supported)" ) {
    nes.bus.write(0x0012, 0x0F);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x10 (BPL rel)" ) {
    nes.bus.write(0x00E2, 0x10);  // BPL rel
    nes.bus.write(0x00E4, 0x00);  // BRK, should not be executed
    nes.cpu.program_counter = 0x00E2;

    SECTION( "No branch" ) {
      nes.bus.write(0x00E3, 0xE6);  // Offset
      nes.cpu.flag |= 0x80;

      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.cpu.program_counter == 0x00E4 );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }

    SECTION( "Branch, same page" ) {
      nes.bus.write(0x00E3, 0xE6);  // Offset
      nes.cpu.flag &= ~0x80;

      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.cpu.program_counter == 0x00CA );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }

    SECTION( "Branch, page crossing" ) {
      nes.bus.write(0x00E3, 0x2E);  // Offset
      nes.cpu.flag &= ~0x80;

      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.cpu.program_counter == 0x0112 );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }
  }

  SECTION( "0x11 (ORA ind,Y)" ) {
    nes.bus.write(0x0012, 0x11);  // ORA ind,Y
    nes.bus.write(0x0014, 0x00);  // BRK, should not be executed

    SECTION( "Same page" ) {
      nes.bus.write(0x0013, 0x0E);  // Lower address byte
      nes.bus.write(0x000E, 0x35);  // Lower address byte
      nes.bus.write(0x000F, 0x01);  // Upper address byte
      nes.bus.write(0x0137, 0x42);  // Operand

      nes.cpu.program_counter = 0x0012;
      nes.cpu.y_index = 0x02;
      nes.cpu.accumulator = 0xCC;

      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.cpu.accumulator == 0xCE );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

      REQUIRE( nes.cpu.program_counter == 0x0014 );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }

    SECTION( "Page crossing" ) {
      nes.bus.write(0x0013, 0x0E);  // Lower address byte
      nes.bus.write(0x000E, 0x35);  // Lower address byte
      nes.bus.write(0x000F, 0x01);  // Upper address byte
      nes.bus.write(0x0212, 0x42);  // Operand

      nes.cpu.program_counter = 0x0012;
      nes.cpu.y_index = 0xDD;
      nes.cpu.accumulator = 0xCC;

      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.cpu.accumulator == 0xCE );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

      REQUIRE( nes.cpu.program_counter == 0x0014 );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }
  }

  SECTION( "0x12 (Not supported)" ) {
    nes.bus.write(0x0012, 0x12);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x13 (Not supported)" ) {
    nes.bus.write(0x0012, 0x13);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x14 (Not supported)" ) {
    nes.bus.write(0x0012, 0x14);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x15 (ORA zpg,X)" ) {
    nes.bus.write(0x0012, 0x15);  // ORA zpg,X
    nes.bus.write(0x0013, 0x32);  // Lower byte
    nes.bus.write(0x0014, 0x00);  // BRK, should not be executed
    nes.bus.write(0x0035, 0x42);  // Operand

    nes.cpu.x_index = 0x03;
    nes.cpu.program_counter = 0x0012;
    nes.cpu.accumulator = 0xCC;

    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.cpu.accumulator == 0xCE );
    REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
    REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

    REQUIRE( nes.cpu.program_counter == 0x0014 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x16 (ASL zpg,X)" ) {
    nes.bus.write(0x0012, 0x16);  // ASL zpg,X
    nes.bus.write(0x0013, 0x0C);  // Lower address byte
    nes.bus.write(0x0014, 0x00);  // BRK, should not be executed
    nes.bus.write(0x000E, 0x42);  // Opperand

    nes.cpu.x_index = 0x02;
    nes.cpu.program_counter = 0x0012;

    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( nes.bus.read(0x000E) == 0x84 );
    REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
    REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

    REQUIRE( nes.cpu.program_counter == 0x0014 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x17 (Not supported)" ) {
    nes.bus.write(0x0012, 0x17);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x18 (CLC impl)" ) {
    nes.bus.write(0x0012, 0x18);  // CLC impl
    nes.bus.write(0x0013, 0x00);  // BRK, should not be executed

    nes.cpu.program_counter = 0x12;
    nes.cpu.flag |= 0x01;

    nes.cpu.tick();
    nes.cpu.tick();

    REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );
    REQUIRE( nes.cpu.program_counter == 0x0013 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "0x19 (ORA abs,Y)" ) {
    nes.bus.write(0x0012, 0x19);  // ORA abs,Y
    nes.bus.write(0x0015, 0x00);  // BRK, should not be executed
    nes.cpu.program_counter = 0x0012;
    nes.cpu.accumulator = 0xCC;

    SECTION( "Same page" ) {
      nes.bus.write(0x0013, 0x32);  // Lower byte
      nes.bus.write(0x0014, 0x01);  // Upper byte
      nes.bus.write(0x0135, 0x42);  // Operand

      nes.cpu.y_index = 0x03;

      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.cpu.accumulator == 0xCE );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

      REQUIRE( nes.cpu.program_counter == 0x0015 );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }

    SECTION( "Page crossing" ) {
      nes.bus.write(0x0013, 0xCD);  // Lower byte
      nes.bus.write(0x0014, 0x00);  // Upper byte
      nes.bus.write(0x0135, 0x42);  // Operand

      nes.cpu.y_index = 0x68;

      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.cpu.accumulator == 0xCE );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

      REQUIRE( nes.cpu.program_counter == 0x0015 );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }
  }

  SECTION( "0x1A (Not supported)" ) {
    nes.bus.write(0x0012, 0x1A);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x1B (Not supported)" ) {
    nes.bus.write(0x0012, 0x1B);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x1C (Not supported)" ) {
    nes.bus.write(0x0012, 0x1C);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }

  SECTION( "0x1D (ORA abs,X)" ) {
    nes.bus.write(0x0012, 0x1D);  // ORA abs,X
    nes.bus.write(0x0015, 0x00);  // BRK, should not be executed
    nes.cpu.program_counter = 0x0012;
    nes.cpu.accumulator = 0xCC;

    SECTION( "Same page" ) {
      nes.bus.write(0x0013, 0x32);  // Lower byte
      nes.bus.write(0x0014, 0x01);  // Upper byte
      nes.bus.write(0x0135, 0x42);  // Operand

      nes.cpu.x_index = 0x03;

      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.cpu.accumulator == 0xCE );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

      REQUIRE( nes.cpu.program_counter == 0x0015 );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }

    SECTION( "Page crossing" ) {
      nes.bus.write(0x0013, 0xCD);  // Lower byte
      nes.bus.write(0x0014, 0x00);  // Upper byte
      nes.bus.write(0x0135, 0x42);  // Operand

      nes.cpu.x_index = 0x68;

      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.cpu.accumulator == 0xCE );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

      REQUIRE( nes.cpu.program_counter == 0x0015 );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }
  }

  SECTION( "0x1E (ASL abs,X)" ) {
    nes.bus.write(0x0012, 0x1E);  // ASL abs,X
    nes.bus.write(0x0015, 0x00);  // BRK, should not be executed
    nes.cpu.program_counter = 0x0012;

    SECTION( "Same page" ) {
      nes.bus.write(0x0013, 0x32);  // Lower byte
      nes.bus.write(0x0014, 0x01);  // Upper byte
      nes.bus.write(0x0135, 0x42);  // Operand

      nes.cpu.x_index = 0x03;

      // According to specifications, same page takes 7 cycles. Due to implementation specifics, it is implemented here as 6 instead
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.bus.read(0x000E) == 0x84 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

      REQUIRE( nes.cpu.program_counter == 0x0015 );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }

    SECTION( "Page crossing" ) {
      nes.bus.write(0x0013, 0xCD);  // Lower byte
      nes.bus.write(0x0014, 0x00);  // Upper byte
      nes.bus.write(0x0135, 0x42);  // Operand

      nes.cpu.x_index = 0x68;

      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();
      nes.cpu.tick();

      REQUIRE( nes.bus.read(0x000E) == 0x84 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );

      REQUIRE( nes.cpu.program_counter == 0x0015 );
      REQUIRE( nes.cpu.lockout_counter == 0 );
    }
  }

  SECTION( "0x1F (Not supported)" ) {
    nes.bus.write(0x0012, 0x1F);
    nes.cpu.program_counter = 0x0012;

    REQUIRE_THROWS_AS(nes.cpu.tick(), std::runtime_error);
  }
}
