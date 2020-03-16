set(SRC
  main.cc

  base.cc
  bfs-path.cc
  bipartite.cc
  cc.cc
  cycle.cc
  dfs.cc
  dfs-path.cc
  graph.cc

  test_base.cc
  test_bfs_path.cc
  test_bipartite.cc
  test_cc.cc
  test_cycle.cc
  test_dfs.cc
  test_dfs_path.cc
  test_graph.cc
)
set(TEST_NAME utest_balgosrbkw_04_undirected.bin)


add_executable(${TEST_NAME} ${SRC})

add_custom_target(check-balgosrbkw-04-undirected
  COMMAND $<TARGET_FILE:${TEST_NAME}>
  DEPENDS ${TEST_NAME}
)

add_dependencies(build-tests ${TEST_NAME})
