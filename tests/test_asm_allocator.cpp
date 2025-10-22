#include <catch2/catch_test_macros.hpp>

#include "allocator.h"

#include <cstdint>
#include <optional>

TEST_CASE( "Assembler allocator", "[Allocator]" ) {
  Allocator alloc = Allocator();

  SECTION( "Reserve start of block" ) {
    std::optional<uint16_t> loc = alloc.reserve_at(0x0200, 1);

    REQUIRE( loc == 0x0200 );
    REQUIRE( alloc.memory_blocks[0].first == 0x0000 );
    REQUIRE( alloc.memory_blocks[0].second == 0x00FF );
    REQUIRE( alloc.memory_blocks[1].first == 0x0201 );
    REQUIRE( alloc.memory_blocks[1].second == 0x07FF );
  }

  SECTION( "Reserve end of block" ) {
    std::optional<uint16_t> loc = alloc.reserve_at(0x00FF, 1);

    REQUIRE( loc == 0x00FF );
    REQUIRE( alloc.memory_blocks[0].first == 0x0000 );
    REQUIRE( alloc.memory_blocks[0].second == 0x00FE );
    REQUIRE( alloc.memory_blocks[1].first == 0x0200 );
    REQUIRE( alloc.memory_blocks[1].second == 0x07FF );
  }

  SECTION( "Reserve middle of block" ) {
    std::optional<uint16_t> loc = alloc.reserve_at(0x00A0, 1);

    REQUIRE( loc == 0x00A0 );
    REQUIRE( alloc.memory_blocks[0].first == 0x0000 );
    REQUIRE( alloc.memory_blocks[0].second == 0x009F );
    REQUIRE( alloc.memory_blocks[1].first == 0x00A1 );
    REQUIRE( alloc.memory_blocks[1].second == 0x00FF );
    REQUIRE( alloc.memory_blocks[2].first == 0x0200 );
    REQUIRE( alloc.memory_blocks[2].second == 0x07FF );
  }
}
