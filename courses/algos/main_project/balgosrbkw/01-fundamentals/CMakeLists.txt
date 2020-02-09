add_subdirectory(binary-search)
add_subdirectory(queuell)
add_subdirectory(stack)
add_subdirectory(stackfixed)
add_subdirectory(stackll)
add_subdirectory(unionfind)


add_custom_target(check-balgosrbkw-01
   DEPENDS check-balgosrbkw-01-binsearch check-balgosrbkw-01-queuell check-balgosrbkw-01-stack check-balgosrbkw-01-stackfixed check-balgosrbkw-01-stackll check-balgosrbkw-01-unionfind
)
