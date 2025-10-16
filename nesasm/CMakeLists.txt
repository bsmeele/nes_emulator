add_executable(nesasm
  main.cpp
  ast.cpp
)
target_link_libraries(nesasm PUBLIC shared)
