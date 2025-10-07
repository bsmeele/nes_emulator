cmake_minimum_required(VERSION 3.28)
project(NES_emulator)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(TESTING "Build and run test" OFF)

add_library(nescore
  src/nes.cpp
  src/cpu.cpp
  src/address_modes.cpp
  src/operations.cpp
  src/bus.cpp
  src/ram.cpp
  src/cartridge.cpp
  src/nrom.cpp
)

target_include_directories(nescore PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (TESTING)
  target_compile_definitions(nescore PUBLIC TESTING)
endif()

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nescore)

if (TESTING)
  Include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.8.1
  )

  FetchContent_MakeAvailable(Catch2)

  enable_testing()

  add_executable(tests
    tests/tests.cpp
    tests/test_cpu_address_mode.cpp
    tests/test_cpu_operation.cpp
  )
  target_link_libraries(tests PRIVATE nescore Catch2::Catch2WithMain)

  include(Catch)
  catch_discover_tests(tests)
endif()
