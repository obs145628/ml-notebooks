add_subdirectory(undirected)

add_custom_target(check-balgosrbkw-04
   DEPENDS check-balgosrbkw-04-undirected
)
