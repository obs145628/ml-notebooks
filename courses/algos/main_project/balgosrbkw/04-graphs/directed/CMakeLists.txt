set(SRC
  main.cc

  dfs-order.cc
  digraph.cc
  directed-cycle.cc
  directed-dfs.cc
  examples.cc
  scc.cc
  topological.cc
  transitive-closure.cc

  test_dfs_order.cc
  test_digraph.cc
  test_directed_cycle.cc
  test_directed_dfs.cc
  test_scc.cc
  test_topological.cc
  test_transitive_closure.cc
)
set(TEST_NAME utest_balgosrbkw_04_directed.bin)


add_executable(${TEST_NAME} ${SRC})

add_custom_target(check-balgosrbkw-04-directed
  COMMAND $<TARGET_FILE:${TEST_NAME}>
  DEPENDS ${TEST_NAME}
)


add_dependencies(build-tests ${TEST_NAME})
