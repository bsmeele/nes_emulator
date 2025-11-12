add_library(nescore
  nes.cpp
  cpu/cpu.cpp
  cpu/address_modes.cpp
  cpu/operations.cpp
  ppu/ppu.cpp
  ppu/sprite_unit.cpp
  bus.cpp
  ram.cpp
  cartridge/cartridge.cpp
  cartridge/nrom.cpp
)
target_include_directories(nescore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(nescore PUBLIC shared)

if (TESTING)
  target_compile_definitions(nescore PUBLIC TESTING)
endif()

add_executable(nesemu main.cpp)
target_link_libraries(nesemu PRIVATE nescore)
