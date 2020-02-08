set(SRC
  src/ledebug.c
)
add_library(ledebug ${SRC})
target_link_libraries(ledebug lestd)
