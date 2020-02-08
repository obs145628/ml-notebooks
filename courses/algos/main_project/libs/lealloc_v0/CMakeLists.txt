set(SRC
  src/alloc.c
)
add_library(lealloc_v0 ${SRC})
target_link_libraries(lealloc_v0 lestd)
