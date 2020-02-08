set(SRC
  src/alloc.c
)
add_library(lealloc_v01 ${SRC})
target_link_libraries(lealloc_v01 lestd)
