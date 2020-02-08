set(SRC
  main.c
  sort.c
)
set(TEST_NAME test_balgosrbkw_02_selectionsort)


add_executable(${TEST_NAME} ${SRC})
target_link_libraries(${TEST_NAME} ledebug leio lealloc_v0 lerand)


add_custom_target(check-balgosrbkw-02-selectionsort
  COMMAND python3 ${CMAKE_SOURCE_DIR}/tests/checker.py
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TEST_NAME}
  ${PROJECT_BINARY_DIR}
  DEPENDS ${TEST_NAME}
)
