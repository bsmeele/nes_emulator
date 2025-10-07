#include <catch2/catch_test_macros.hpp>

#include "nes.h"

TEST_CASE( "CPU operations", "[CPU]" ) {
  NES nes = NES();

  SECTION( "ADC" ) {
    SECTION( "Base" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.accumulator = 0x34;

      nes.cpu.ADC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x46 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
    SECTION( "Base + carry" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.accumulator = 0x34;
      nes.cpu.flag |= 0x01;

      nes.cpu.ADC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x47 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Carry flag" ) {
      nes.bus.write(0x0001, 0x69);
      nes.cpu.accumulator = 0xCD;

      nes.cpu.ADC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x36 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.bus.write(0x0001, 0xEF);
      nes.cpu.accumulator = 0x11;

      nes.cpu.ADC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Overflow flag" ) {
      nes.bus.write(0x0001, 0xCD);
      nes.cpu.accumulator = 0x82;

      nes.cpu.ADC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x4F );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x40 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.bus.write(0x0001, 0xCD);
      nes.cpu.accumulator = 0x12;

      nes.cpu.ADC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0xDF );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "AND" ) {
    SECTION( "Base" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.accumulator = 0x34;

      nes.cpu.AND(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x10 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.accumulator = 0xED;

      nes.cpu.AND(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.bus.write(0x0001, 0xFA);
      nes.cpu.accumulator = 0xAF;

      nes.cpu.AND(0x0001);

      REQUIRE( nes.cpu.accumulator == 0xAA );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "ASL" ) {
    SECTION( "Base" ) {
      nes.cpu.accumulator = 0x35;

      nes.cpu.ASL({});

      REQUIRE( nes.cpu.accumulator == 0x6A );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Carry flag" ) {
      nes.cpu.accumulator = 0x99;

      nes.cpu.ASL({});

      REQUIRE( nes.cpu.accumulator == 0x32 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.cpu.accumulator = 0x80;

      nes.cpu.ASL({});

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.cpu.accumulator = 0x55;

      nes.cpu.ASL({});

      REQUIRE( nes.cpu.accumulator == 0xAA );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Memory" ) {
      nes.bus.write(0x0001, 0x34);

      nes.cpu.ASL(0x0001);

      REQUIRE( nes.bus.read(0x0001) == 0x68 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "BCC" ) {
    SECTION( "No branch" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;
      nes.cpu.flag |= 0x01;

      nes.cpu.BCC(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0034 );
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Branch same page" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;

      nes.cpu.BCC(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0046 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing" ) {
      nes.bus.write(0x0001, 0x34);
      nes.cpu.program_counter = 0x00E8;

      nes.cpu.BCC(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x11C );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Branch same page negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x00E8;

      nes.cpu.BCC(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00B6 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x0123;

      nes.cpu.BCC(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00F1 );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "BCS" ) {
    SECTION( "No branch" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;

      nes.cpu.BCS(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0034 );
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Branch same page" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;
      nes.cpu.flag |= 0x01;

      nes.cpu.BCS(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0046 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing" ) {
      nes.bus.write(0x0001, 0x34);
      nes.cpu.program_counter = 0x00E8;
      nes.cpu.flag |= 0x01;

      nes.cpu.BCS(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x11C );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Branch same page negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x00E8;
      nes.cpu.flag |= 0x01;

      nes.cpu.BCS(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00B6 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x0123;
      nes.cpu.flag |= 0x01;

      nes.cpu.BCS(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00F1 );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "BEQ" ) {
    SECTION( "No branch" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;

      nes.cpu.BEQ(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0034 );
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Branch same page" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;
      nes.cpu.flag |= 0x02;

      nes.cpu.BEQ(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0046 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing" ) {
      nes.bus.write(0x0001, 0x34);
      nes.cpu.program_counter = 0x00E8;
      nes.cpu.flag |= 0x02;

      nes.cpu.BEQ(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x11C );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Branch same page negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x00E8;
      nes.cpu.flag |= 0x02;

      nes.cpu.BEQ(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00B6 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x0123;
      nes.cpu.flag |= 0x02;

      nes.cpu.BEQ(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00F1 );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "BIT" ) {
    SECTION( "Base" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.accumulator = 0x34;

      nes.cpu.BIT(0x0001);

      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0xC0) == 0x00 );  // Overflow and negative flags
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.accumulator = 0x24;

      nes.cpu.BIT(0x0001);

      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0xC0) == 0x00 );  // Overflow and negative flags
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Overflow and negative flag" ) {
      nes.bus.write(0x0001, 0x4A);
      nes.cpu.accumulator = 0x69;

      nes.cpu.BIT(0x0001);

      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0xC0) == 0x40 );  // Overflow and negative flags
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "BMI" ) {
    SECTION( "No branch" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;

      nes.cpu.BMI(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0034 );
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Branch same page" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;
      nes.cpu.flag |= 0x80;

      nes.cpu.BMI(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0046 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing" ) {
      nes.bus.write(0x0001, 0x34);
      nes.cpu.program_counter = 0x00E8;
      nes.cpu.flag |= 0x80;

      nes.cpu.BMI(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x11C );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Branch same page negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x00E8;
      nes.cpu.flag |= 0x80;

      nes.cpu.BMI(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00B6 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x0123;
      nes.cpu.flag |= 0x80;

      nes.cpu.BMI(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00F1 );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "BNE" ) {
    SECTION( "No branch" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;
      nes.cpu.flag |= 0x02;

      nes.cpu.BNE(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0034 );
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Branch same page" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;

      nes.cpu.BNE(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0046 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing" ) {
      nes.bus.write(0x0001, 0x34);
      nes.cpu.program_counter = 0x00E8;

      nes.cpu.BNE(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x11C );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Branch same page negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x00E8;

      nes.cpu.BNE(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00B6 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x0123;

      nes.cpu.BNE(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00F1 );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "BPL" ) {
    SECTION( "No branch" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;
      nes.cpu.flag |= 0x80;

      nes.cpu.BPL(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0034 );
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Branch same page" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;

      nes.cpu.BPL(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0046 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing" ) {
      nes.bus.write(0x0001, 0x34);
      nes.cpu.program_counter = 0x00E8;

      nes.cpu.BPL(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x11C );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Branch same page negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x00E8;

      nes.cpu.BPL(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00B6 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x0123;

      nes.cpu.BPL(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00F1 );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "BRK" ) {
    nes.cpu.program_counter = 0x1233;
    nes.cpu.stack_pointer = 0x56;
    nes.cpu.flag &= ~0x04;

    nes.cpu.BRK({});

    REQUIRE( nes.bus.read(0x0156) == 0x12 );
    REQUIRE( nes.bus.read(0x0155) == 0x34 );
    REQUIRE( nes.bus.read(0x0154) == 0x30 );
    REQUIRE( nes.cpu.program_counter == 0xFFFE );
    REQUIRE( nes.cpu.flag == 0x24 );  // Interrupt disable flag
    REQUIRE( nes.cpu.stack_pointer == 0x53 );
    REQUIRE( nes.cpu.lockout_counter == 6 );
  }

  SECTION( "BVC" ) {
    SECTION( "No branch" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;
      nes.cpu.flag |= 0x40;

      nes.cpu.BVC(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0034 );
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Branch same page" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;

      nes.cpu.BVC(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0046 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing" ) {
      nes.bus.write(0x0001, 0x34);
      nes.cpu.program_counter = 0x00E8;

      nes.cpu.BVC(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x11C );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Branch same page negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x00E8;

      nes.cpu.BVC(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00B6 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x0123;

      nes.cpu.BVC(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00F1 );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "BVS" ) {
    SECTION( "No branch" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;

      nes.cpu.BVS(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0034 );
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Branch same page" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.program_counter = 0x0034;
      nes.cpu.flag |= 0x40;

      nes.cpu.BVS(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x0046 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing" ) {
      nes.bus.write(0x0001, 0x34);
      nes.cpu.program_counter = 0x00E8;
      nes.cpu.flag |= 0x40;

      nes.cpu.BVS(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x11C );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Branch same page negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x00E8;
      nes.cpu.flag |= 0x40;

      nes.cpu.BVS(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00B6 );
      REQUIRE( nes.cpu.lockout_counter == 2 );
    }

    SECTION( "Branch page crossing negative offset" ) {
      nes.bus.write(0x0001, 0xCE);
      nes.cpu.program_counter = 0x0123;
      nes.cpu.flag |= 0x40;

      nes.cpu.BVS(0x0001);

      REQUIRE( nes.cpu.program_counter == 0x00F1 );
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "CLC" ) {
    nes.cpu.flag |= 0x01;

    nes.cpu.CLC({});

    REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "CLD" ) {
    nes.cpu.flag |= 0x08;

    nes.cpu.CLD({});

    REQUIRE( (nes.cpu.flag & 0x08) == 0x00 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "CLI" ) {
    nes.cpu.flag |= 0x04;

    nes.cpu.CLI({});

    REQUIRE( (nes.cpu.flag & 0x04) == 0x00 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "CLV" ) {
    nes.cpu.flag |= 0x40;

    nes.cpu.CLV({});

    REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "CMP" ) {
    nes.bus.write(0x0001, 0x12);

    SECTION( "Greater equal" ) {
      nes.cpu.accumulator = 0x34;

      nes.cpu.CMP(0x0001);

      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Equal" ) {
      nes.cpu.accumulator = 0x12;

      nes.cpu.CMP(0x0001);

      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Less than" ) {
      nes.cpu.accumulator = 0x11;

      nes.cpu.CMP(0x0001);

      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "CPX" ) {
    nes.bus.write(0x0001, 0x12);

    SECTION( "Greater equal" ) {
      nes.cpu.x_index = 0x34;

      nes.cpu.CPX(0x0001);

      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Equal" ) {
      nes.cpu.x_index = 0x12;

      nes.cpu.CPX(0x0001);

      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Less than" ) {
      nes.cpu.x_index = 0x11;

      nes.cpu.CPX(0x0001);

      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "CPY" ) {
    nes.bus.write(0x0001, 0x12);

    SECTION( "Greater equal" ) {
      nes.cpu.y_index = 0x34;

      nes.cpu.CPY(0x0001);

      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Equal" ) {
      nes.cpu.y_index = 0x12;

      nes.cpu.CPY(0x0001);

      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Less than" ) {
      nes.cpu.y_index = 0x11;

      nes.cpu.CPY(0x0001);

      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "DEC" ) {
    SECTION( "Base" ) {
      nes.bus.write(0x0001, 0x12);

      nes.cpu.DEC(0x0001);

      REQUIRE( nes.bus.read(0x0001) == 0x11 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Zero flag" ) {
      nes.bus.write(0x0001, 0x01);

      nes.cpu.DEC(0x0001);

      REQUIRE( nes.bus.read(0x0001) == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Negative flag" ) {
      nes.bus.write(0x0001, 0xCD);

      nes.cpu.DEC(0x0001);

      REQUIRE( nes.bus.read(0x0001) == 0xCC );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "DEX" ) {
    SECTION( "Base" ) {
      nes.cpu.x_index = 0x12;

      nes.cpu.DEX({});

      REQUIRE( nes.cpu.x_index == 0x11 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.cpu.x_index = 0x01;

      nes.cpu.DEX({});

      REQUIRE( nes.cpu.x_index == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.cpu.x_index = 0xCD;

      nes.cpu.DEX({});

      REQUIRE( nes.cpu.x_index == 0xCC );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "DEY" ) {
    SECTION( "Base" ) {
      nes.cpu.y_index = 0x12;

      nes.cpu.DEY({});

      REQUIRE( nes.cpu.y_index == 0x11 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.cpu.y_index = 0x01;

      nes.cpu.DEY({});

      REQUIRE( nes.cpu.y_index == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.cpu.y_index = 0xCD;

      nes.cpu.DEY({});

      REQUIRE( nes.cpu.y_index == 0xCC );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "EOR" ) {
    SECTION( "Base" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.accumulator = 0x34;

      nes.cpu.EOR(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x26);
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.bus.write(0x0001, 0x55);
      nes.cpu.accumulator = 0x55;

      nes.cpu.EOR(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x00);
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.bus.write(0x0001, 0x55);
      nes.cpu.accumulator = 0xE2;

      nes.cpu.EOR(0x0001);

      REQUIRE( nes.cpu.accumulator == 0xB7);
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "INC" ) {
    SECTION( "Base" ) {
      nes.bus.write(0x0001, 0x12);

      nes.cpu.INC(0x0001);

      REQUIRE( nes.bus.read(0x0001) == 0x13 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Zero flag" ) {
      nes.bus.write(0x0001, 0xFF);

      nes.cpu.INC(0x0001);

      REQUIRE( nes.bus.read(0x0001) == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Negative flag" ) {
      nes.bus.write(0x0001, 0xCD);

      nes.cpu.INC(0x0001);

      REQUIRE( nes.bus.read(0x0001) == 0xCE );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "INX" ) {
    SECTION( "Base" ) {
      nes.cpu.x_index = 0x12;

      nes.cpu.INX({});

      REQUIRE( nes.cpu.x_index == 0x13 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.cpu.x_index = 0xFF;

      nes.cpu.INX({});

      REQUIRE( nes.cpu.x_index == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.cpu.x_index = 0xCD;

      nes.cpu.INX({});

      REQUIRE( nes.cpu.x_index == 0xCE );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "INY" ) {
    SECTION( "Base" ) {
      nes.cpu.y_index = 0x12;

      nes.cpu.INY({});

      REQUIRE( nes.cpu.y_index == 0x13 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.cpu.y_index = 0xFF;

      nes.cpu.INY({});

      REQUIRE( nes.cpu.y_index == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.cpu.y_index = 0xCD;

      nes.cpu.INY({});

      REQUIRE( nes.cpu.y_index == 0xCE );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "JMP" ) {
    nes.cpu.program_counter = 0x0001;

    nes.cpu.JMP(0x1234);

    REQUIRE( nes.cpu.program_counter == 0x1234 );
    REQUIRE( nes.cpu.lockout_counter == 0 );
  }

  SECTION( "JSR" ) {
    nes.cpu.program_counter = 0x1234;
    nes.cpu.stack_pointer = 0x56;

    nes.cpu.JSR(0x4321);

    REQUIRE( nes.cpu.program_counter == 0x4321 );
    REQUIRE( nes.cpu.stack_pointer == 0x54 );
    REQUIRE( nes.bus.read(0x0156) == 0x12 );
    REQUIRE( nes.bus.read(0x0155) == 0x34 );
    REQUIRE( nes.cpu.lockout_counter == 3 );
  }

  SECTION( "LDA" ) {
    nes.cpu.accumulator = 0x00;

    SECTION( "Base" ) {
      nes.bus.write(0x0001, 0x12);

      nes.cpu.LDA(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x12 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.bus.write(0x0001, 0x00);

      nes.cpu.LDA(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.bus.write(0x0001, 0xCD);

      nes.cpu.LDA(0x0001);

      REQUIRE( nes.cpu.accumulator == 0xCD );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "LDX" ) {
    nes.cpu.x_index = 0x00;

    SECTION( "Base" ) {
      nes.bus.write(0x0001, 0x12);

      nes.cpu.LDX(0x0001);

      REQUIRE( nes.cpu.x_index == 0x12 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.bus.write(0x0001, 0x00);

      nes.cpu.LDX(0x0001);

      REQUIRE( nes.cpu.x_index == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.bus.write(0x0001, 0xCD);

      nes.cpu.LDX(0x0001);

      REQUIRE( nes.cpu.x_index== 0xCD );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "LDY" ) {
    nes.cpu.y_index = 0x00;

    SECTION( "Base" ) {
      nes.bus.write(0x0001, 0x12);

      nes.cpu.LDY(0x0001);

      REQUIRE( nes.cpu.y_index == 0x12 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.bus.write(0x0001, 0x00);

      nes.cpu.LDY(0x0001);

      REQUIRE( nes.cpu.y_index == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.bus.write(0x0001, 0xCD);

      nes.cpu.LDY(0x0001);

      REQUIRE( nes.cpu.y_index == 0xCD );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "LSR" ) {
    SECTION( "Base" ) {
      nes.cpu.accumulator = 0x36;

      nes.cpu.LSR({});

      REQUIRE( nes.cpu.accumulator == 0x1B );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Carry flag" ) {
      nes.cpu.accumulator = 0x35;

      nes.cpu.LSR({});

      REQUIRE( nes.cpu.accumulator == 0x1A );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.cpu.accumulator = 0x01;

      nes.cpu.LSR({});

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Memory" ) {
      nes.bus.write(0x0001, 0x34);

      nes.cpu.LSR(0x0001);

      REQUIRE( nes.bus.read(0x0001) == 0x1A );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "NOP" ) {
    uint8_t prev_acc = nes.cpu.accumulator;
    uint8_t prev_x = nes.cpu.x_index;
    uint8_t prev_y = nes.cpu.y_index;
    uint8_t prev_sp = nes.cpu.stack_pointer;
    uint8_t prev_flag = nes.cpu.flag;
    uint16_t prev_pc = nes.cpu.program_counter;

    nes.cpu.NOP({});

    REQUIRE( prev_acc == nes.cpu.accumulator );
    REQUIRE( prev_x == nes.cpu.x_index );
    REQUIRE( prev_y == nes.cpu.y_index );
    REQUIRE( prev_sp == nes.cpu.stack_pointer );
    REQUIRE( prev_flag == nes.cpu.flag );
    REQUIRE( prev_pc == nes.cpu.program_counter );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "PHA" ) {
    nes.cpu.accumulator = 0x12;
    nes.cpu.stack_pointer = 0x34;

    nes.cpu.PHA({});

    REQUIRE( nes.bus.read(0x0134) == 0x12 );
    REQUIRE( nes.cpu.stack_pointer == 0x33 );
    REQUIRE( nes.cpu.lockout_counter == 2 );
  }

  SECTION( "PHP" ) {
    nes.cpu.flag = 0x96;
    nes.cpu.stack_pointer = 0x34;

    nes.cpu.PHP({});

    REQUIRE( nes.bus.read(0x0134) == 0xB6 );
    REQUIRE( nes.cpu.stack_pointer == 0x33 );
    REQUIRE( nes.cpu.lockout_counter == 2 );
  }

  SECTION( "PLA" ) {
    SECTION( "Base" ) {
      nes.cpu.accumulator = 0x00;
      nes.cpu.stack_pointer = 0x12;
      nes.bus.write(0x0113, 0x34);

      nes.cpu.PLA({});

      REQUIRE( nes.cpu.accumulator == 0x34 );
      REQUIRE( nes.cpu.stack_pointer == 0x13 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Zero flag" ) {
      nes.cpu.accumulator = 0x34;
      nes.cpu.stack_pointer = 0x12;
      nes.bus.write(0x0113, 0x00);

      nes.cpu.PLA({});

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( nes.cpu.stack_pointer == 0x13 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }

    SECTION( "Negative flag" ) {
      nes.cpu.accumulator = 0x00;
      nes.cpu.stack_pointer = 0x12;
      nes.bus.write(0x0113, 0xCD);

      nes.cpu.PLA({});

      REQUIRE( nes.cpu.accumulator == 0xCD );
      REQUIRE( nes.cpu.stack_pointer == 0x13 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "PLP" ) {
    nes.cpu.flag = 0xC3;
    nes.cpu.stack_pointer = 0x12;
    nes.bus.write(0x0113, 0x75);

    nes.cpu.PLP({});

    REQUIRE( nes.cpu.flag == 0x55 );
    REQUIRE( nes.cpu.stack_pointer == 0x13 );
    REQUIRE( nes.cpu.lockout_counter == 3);
  }

  SECTION( "ROL" ) {
    SECTION( "Base" ) {
      nes.cpu.flag |= 0x01;
      nes.cpu.accumulator= 0x12;

      nes.cpu.ROL({});

      REQUIRE( nes.cpu.accumulator == 0x25 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Carry flag" ) {
      nes.cpu.flag &= ~0x01;
      nes.cpu.accumulator= 0xA8;

      nes.cpu.ROL({});

      REQUIRE( nes.cpu.accumulator == 0x50 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.cpu.flag &= ~0x01;
      nes.cpu.accumulator= 0x80;

      nes.cpu.ROL({});

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.cpu.flag &= ~0x01;
      nes.cpu.accumulator= 0x69;

      nes.cpu.ROL({});

      REQUIRE( nes.cpu.accumulator == 0xD2 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Memory" ) {
      nes.cpu.flag &= ~0x01;
      nes.bus.write(0x0001, 0x42);

      nes.cpu.ROL(0x0001);

      REQUIRE( nes.bus.read(0x0001) == 0x84 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "ROR" ) {
    SECTION( "Base" ) {
      nes.cpu.flag &= ~0x01;
      nes.cpu.accumulator= 0x12;

      nes.cpu.ROR({});

      REQUIRE( nes.cpu.accumulator == 0x09 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Carry flag" ) {
      nes.cpu.flag &= ~0x01;
      nes.cpu.accumulator= 0xA9;

      nes.cpu.ROR({});

      REQUIRE( nes.cpu.accumulator == 0x54 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.cpu.flag &= ~0x01;
      nes.cpu.accumulator= 0x01;

      nes.cpu.ROR({});

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.cpu.flag |= 0x01;
      nes.cpu.accumulator= 0x69;

      nes.cpu.ROR({});

      REQUIRE( nes.cpu.accumulator == 0xB4 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Memory" ) {
      nes.cpu.flag &= ~0x01;
      nes.bus.write(0x0001, 0x42);

      nes.cpu.ROR(0x0001);

      REQUIRE( nes.bus.read(0x0001) == 0x21 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 3 );
    }
  }

  SECTION( "RTI" ) {
    nes.cpu.program_counter = 0x4321;
    nes.cpu.stack_pointer = 0x56;
    nes.cpu.flag = 0x10;
    nes.bus.write(0x0157, 0xB6);
    nes.bus.write(0x0158, 0x34);
    nes.bus.write(0x0159, 0x12);

    nes.cpu.RTI({});

    REQUIRE( nes.cpu.program_counter == 0x1234 );
    REQUIRE( nes.cpu.stack_pointer == 0x59 );
    REQUIRE( nes.cpu.flag == 0x96 );
    REQUIRE( nes.cpu.lockout_counter == 5 );
  }

  SECTION( "RTS" ) {
    nes.cpu.program_counter = 0x4321;
    nes.cpu.stack_pointer = 0x56;
    nes.bus.write(0x0157, 0x34);
    nes.bus.write(0x0158, 0x12);

    nes.cpu.RTS({});

    REQUIRE( nes.cpu.program_counter == 0x1235 );
    REQUIRE( nes.cpu.stack_pointer == 0x58 );
    REQUIRE( nes.cpu.lockout_counter == 5 );
  }

  SECTION( "SBC" ) {
    SECTION( "Base" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.accumulator = 0x34;
      nes.cpu.flag |= 0x01;

      nes.cpu.SBC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x22 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
    SECTION( "Base + carry" ) {
      nes.bus.write(0x0001, 0x12);
      nes.cpu.accumulator = 0x34;
      nes.cpu.flag &= ~0x01;

      nes.cpu.SBC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x21 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Carry flag" ) {
      nes.bus.write(0x0001, 0x20);
      nes.cpu.accumulator = 0x50;
      nes.cpu.flag |= 0x01;

      nes.cpu.SBC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x30 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero flag" ) {
      nes.bus.write(0x0001, 0x11);
      nes.cpu.accumulator = 0x12;
      nes.cpu.flag &= ~0x01;

      nes.cpu.SBC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Overflow flag" ) {
      nes.bus.write(0x0001, 0x01);
      nes.cpu.accumulator = 0x80;
      nes.cpu.flag |= 0x01;

      nes.cpu.SBC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0x7F );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x40 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative flag" ) {
      nes.bus.write(0x0001, 0xD6);
      nes.cpu.accumulator = 0xBB;
      nes.cpu.flag |= 0x01;

      nes.cpu.SBC(0x0001);

      REQUIRE( nes.cpu.accumulator == 0xE5 );
      REQUIRE( (nes.cpu.flag & 0x01) == 0x00 );  // Carry flag
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x40) == 0x00 );  // Overflow flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "SEC" ) {
    nes.cpu.flag &= ~0x01;

    nes.cpu.SEC({});

    REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "SEC" ) {
    nes.cpu.flag &= ~0x01;

    nes.cpu.SEC({});

    REQUIRE( (nes.cpu.flag & 0x01) == 0x01 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "SED" ) {
    nes.cpu.flag &= ~0x08;

    nes.cpu.SED({});

    REQUIRE( (nes.cpu.flag & 0x08) == 0x08 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "SEI" ) {
    nes.cpu.flag &= ~0x04;

    nes.cpu.SEI({});

    REQUIRE( (nes.cpu.flag & 0x04) == 0x04 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "STA" ) {
    nes.cpu.accumulator = 0x12;

    nes.cpu.STA(0x1234);

    REQUIRE( nes.bus.read(0x1234) == 0x12 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "STX" ) {
    nes.cpu.x_index = 0x12;

    nes.cpu.STX(0x1234);

    REQUIRE( nes.bus.read(0x1234) == 0x12 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "STY" ) {
    nes.cpu.y_index = 0x12;

    nes.cpu.STY(0x1234);

    REQUIRE( nes.bus.read(0x1234) == 0x12 );
    REQUIRE( nes.cpu.lockout_counter == 1 );
  }

  SECTION( "TAX" ) {
    SECTION( "Base" ) {
      nes.cpu.accumulator = 0x12;
      nes.cpu.x_index = 0x34;

      nes.cpu.TAX({});

      REQUIRE( nes.cpu.x_index == 0x12 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero" ) {
      nes.cpu.accumulator = 0x00;
      nes.cpu.x_index = 0x34;

      nes.cpu.TAX({});

      REQUIRE( nes.cpu.x_index == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative" ) {
      nes.cpu.accumulator = 0xCD;
      nes.cpu.x_index = 0x34;

      nes.cpu.TAX({});

      REQUIRE( nes.cpu.x_index == 0xCD );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "TAY" ) {
    SECTION( "Base" ) {
      nes.cpu.accumulator = 0x12;
      nes.cpu.y_index = 0x34;

      nes.cpu.TAY({});

      REQUIRE( nes.cpu.y_index == 0x12 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero" ) {
      nes.cpu.accumulator = 0x00;
      nes.cpu.y_index = 0x34;

      nes.cpu.TAY({});

      REQUIRE( nes.cpu.y_index == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative" ) {
      nes.cpu.accumulator = 0xCD;
      nes.cpu.y_index = 0x34;

      nes.cpu.TAY({});

      REQUIRE( nes.cpu.y_index == 0xCD );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "TSX" ) {
    SECTION( "Base" ) {
      nes.cpu.stack_pointer= 0x12;
      nes.cpu.x_index = 0x34;

      nes.cpu.TSX({});

      REQUIRE( nes.cpu.x_index == 0x12 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero" ) {
      nes.cpu.stack_pointer= 0x00;
      nes.cpu.x_index = 0x34;

      nes.cpu.TSX({});

      REQUIRE( nes.cpu.x_index == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative" ) {
      nes.cpu.stack_pointer= 0xCD;
      nes.cpu.x_index = 0x34;

      nes.cpu.TSX({});

      REQUIRE( nes.cpu.x_index == 0xCD );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "TXA" ) {
    SECTION( "Base" ) {
      nes.cpu.x_index = 0x12;
      nes.cpu.accumulator = 0x34;

      nes.cpu.TXA({});

      REQUIRE( nes.cpu.accumulator == 0x12 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero" ) {
      nes.cpu.x_index = 0x00;
      nes.cpu.accumulator = 0x34;

      nes.cpu.TXA({});

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative" ) {
      nes.cpu.x_index = 0xCD;
      nes.cpu.accumulator = 0x34;

      nes.cpu.TXA({});

      REQUIRE( nes.cpu.accumulator == 0xCD );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "TXS" ) {
    SECTION( "Base" ) {
      nes.cpu.x_index= 0x12;
      nes.cpu.stack_pointer = 0x34;

      nes.cpu.TXS({});

      REQUIRE( nes.cpu.stack_pointer == 0x12 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero" ) {
      nes.cpu.x_index= 0x00;
      nes.cpu.stack_pointer = 0x34;

      nes.cpu.TXS({});

      REQUIRE( nes.cpu.stack_pointer == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative" ) {
      nes.cpu.x_index= 0xCD;
      nes.cpu.stack_pointer = 0x34;

      nes.cpu.TXS({});

      REQUIRE( nes.cpu.stack_pointer == 0xCD );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }

  SECTION( "TYA" ) {
    SECTION( "Base" ) {
      nes.cpu.y_index = 0x12;
      nes.cpu.accumulator = 0x34;

      nes.cpu.TYA({});

      REQUIRE( nes.cpu.accumulator == 0x12 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Zero" ) {
      nes.cpu.y_index = 0x00;
      nes.cpu.accumulator = 0x34;

      nes.cpu.TYA({});

      REQUIRE( nes.cpu.accumulator == 0x00 );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x02 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x00 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }

    SECTION( "Negative" ) {
      nes.cpu.y_index = 0xCD;
      nes.cpu.accumulator = 0x34;

      nes.cpu.TYA({});

      REQUIRE( nes.cpu.accumulator == 0xCD );
      REQUIRE( (nes.cpu.flag & 0x02) == 0x00 );  // Zero flag
      REQUIRE( (nes.cpu.flag & 0x80) == 0x80 );  // Negative flag
      REQUIRE( nes.cpu.lockout_counter == 1 );
    }
  }
}
