set(SRC
  main.c
  stack.c
)
set(TEST_NAME test_balgosrbkw_01_stackll.bin)

add_executable(${TEST_NAME} ${SRC})
target_link_libraries(${TEST_NAME} ledebug leio lealloc_v0)
add_dependencies(build-tests ${TEST_NAME})
