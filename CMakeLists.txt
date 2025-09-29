cmake_minimum_required(VERSION 3.28)
project(NES_emulator)

set(CMAKE_CXX_STANDARD 17)

# include_directories(src)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/nes.cpp
  src/cpu.cpp
  src/bus.cpp
  src/ram.cpp
)
