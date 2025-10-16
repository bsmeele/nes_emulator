cmake_minimum_required(VERSION 3.28)
project(nes_emu)

set(CMAKE_CXX_STANDARD 17)

option(TESTING "Build and run test" OFF)

add_subdirectory(lib/shared)
add_subdirectory(nescore)
add_subdirectory(nesasm)
add_subdirectory(tests)
