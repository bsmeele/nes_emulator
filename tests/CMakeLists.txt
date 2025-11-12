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
    tests.cpp
    test_cpu_address_mode.cpp
    test_cpu_operation.cpp
    test_cpu_opcodes.cpp
    test_rom_cpu.cpp
    test_asm_allocator.cpp
    test_assembler.cpp
    test_ppu.cpp
  )
  target_link_libraries(tests PRIVATE nescore)
  target_link_libraries(tests PRIVATE asmcore)
  target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

  include(Catch)
  catch_discover_tests(tests)
endif()
