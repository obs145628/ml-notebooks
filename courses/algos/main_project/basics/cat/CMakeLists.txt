set(SRC
  main.c
)
set(TEST_NAME test_basics_cat)


add_executable(${TEST_NAME} ${SRC})
target_link_libraries(${TEST_NAME} leio)


add_custom_target(check-basics-cat
  COMMAND python3 ${CMAKE_SOURCE_DIR}/tests/checker.py
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TEST_NAME}
  ${PROJECT_BINARY_DIR}
  DEPENDS ${TEST_NAME}
)
