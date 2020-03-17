set(SRC
  main.c
)
set(TEST_NAME test_basics_printer.bin)

add_executable(${TEST_NAME} ${SRC})
target_link_libraries(${TEST_NAME} leio)
add_dependencies(build-tests ${TEST_NAME})
