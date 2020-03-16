set(SRC
  main.c
  sort.c
)
set(TEST_NAME test_balgosrbkw_02_tdmergesort.bin)

add_executable(${TEST_NAME} ${SRC})
target_link_libraries(${TEST_NAME} ledebug leio lealloc_v0 lerand)
add_dependencies(build-tests ${TEST_NAME})
