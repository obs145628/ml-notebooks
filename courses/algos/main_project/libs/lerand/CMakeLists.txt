set(SRC
  src/lerand.c
)
add_library(lerand ${SRC})
target_link_libraries(lerand lealloc_v0)
