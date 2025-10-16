add_library(shared
  address_mode.cpp
  operation.cpp
)
target_include_directories(shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
