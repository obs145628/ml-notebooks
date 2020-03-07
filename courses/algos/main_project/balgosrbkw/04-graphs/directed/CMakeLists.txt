set(SRC
  main.cc

  digraph.cc
  directed-cycle.cc
  directed-dfs.cc

  test_digraph.cc
  test_directed_cycle.cc
  test_directed_dfs.cc
)
set(TEST_NAME test_balgosrbkw_04_directed)


add_executable(${TEST_NAME} ${SRC})

add_custom_target(check-balgosrbkw-04-directed
  COMMAND $<TARGET_FILE:${TEST_NAME}>
  DEPENDS ${TEST_NAME}
)
