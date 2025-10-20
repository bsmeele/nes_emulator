add_library(asmcore
  assembler.cpp
  allocator.cpp
  ast.cpp
  directive.cpp
  utils.cpp
)
target_include_directories(asmcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(asmcore PUBLIC shared)

if (TESTING)
  target_compile_definitions(asmcore PUBLIC TESTING)
endif()


add_executable(nesasm main.cpp)
target_link_libraries(nesasm PRIVATE asmcore)
