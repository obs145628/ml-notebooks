set(SRC
  src/leio.c
)
add_library(leio ${SRC})
target_link_libraries(leio lestd)
