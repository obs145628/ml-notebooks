add_subdirectory(01-fundamentals)
add_subdirectory(02-sorting)
add_subdirectory(03-symtables)

add_custom_target(check-balgosrbkw
   DEPENDS check-balgosrbkw-01 check-balgosrbkw-02 check-balgosrbkw-03
)
