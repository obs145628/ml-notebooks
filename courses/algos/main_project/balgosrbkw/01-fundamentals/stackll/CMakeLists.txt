set(SRC
  main.c
  stack.c
)
set(TEST_NAME test_balgosrbkw_01_stackll)


add_executable(${TEST_NAME} ${SRC})
target_link_libraries(${TEST_NAME} ledebug leio lealloc_v0)


add_custom_target(check-balgosrbkw-01-stackll
  COMMAND python3 ${CMAKE_SOURCE_DIR}/tests/checker.py
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TEST_NAME}
  ${PROJECT_BINARY_DIR}
  DEPENDS ${TEST_NAME}
)
