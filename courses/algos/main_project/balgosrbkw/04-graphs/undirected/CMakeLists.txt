set(SRC
  main.cc
  test_graph.cc
)
set(TEST_NAME test_balgosrbkw_04_undirected)


add_executable(${TEST_NAME} ${SRC})

add_custom_target(check-balgosrbkw-04-undirected
  COMMAND $<TARGET_FILE:${TEST_NAME}>
  DEPENDS ${TEST_NAME}
)
