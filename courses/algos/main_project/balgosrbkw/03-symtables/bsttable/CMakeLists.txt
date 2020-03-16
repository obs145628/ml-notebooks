set(SRC
  main.c
  table.c
)
set(TEST_NAME test_balgosrbkw_03_bsttable.bin)

add_executable(${TEST_NAME} ${SRC})
target_link_libraries(${TEST_NAME} ledebug leio lealloc_v0 lerand)
add_dependencies(build-tests ${TEST_NAME})
