cmake_minimum_required(VERSION 3.28)
project(NES_emulator)

set(CMAKE_CXX_STANDARD 17)

add_library(nescore
  src/nes.cpp
  src/cpu.cpp
  src/bus.cpp
  src/ram.cpp
  src/cartridge.cpp
  src/nrom.cpp
)

target_include_directories(nescore PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nescore)

option(TESTING "Build and run test" OFF)

if (TESTING)
  Include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.8.1
  )

  FetchContent_MakeAvailable(Catch2)

  enable_testing()

  add_executable(tests tests/tests.cpp)
  target_link_libraries(tests PRIVATE nescore Catch2::Catch2WithMain)

  include(Catch)
  catch_discover_tests(tests)
endif()
