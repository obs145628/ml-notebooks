set(SRC
  main.cc

  base.cc
  bfs-path.cc
  dfs.cc
  dfs-path.cc
  graph.cc

  test_base.cc
  test_bfs_path.cc
  test_dfs.cc
  test_dfs_path.cc
  test_graph.cc
)
set(TEST_NAME test_balgosrbkw_04_undirected)


add_executable(${TEST_NAME} ${SRC})

add_custom_target(check-balgosrbkw-04-undirected
  COMMAND $<TARGET_FILE:${TEST_NAME}>
  DEPENDS ${TEST_NAME}
)
