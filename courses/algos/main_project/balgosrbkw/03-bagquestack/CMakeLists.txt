add_subdirectory(stack)
add_subdirectory(stackfixed)
add_subdirectory(stackll)
add_subdirectory(queuell)
add_subdirectory(unionfind)

add_custom_target(check-balgosrbkw-03
   DEPENDS check-balgosrbkw-03-stack check-balgosrbkw-03-stackfixed check-balgosrbkw-03-stackll check-balgosrbkw-03-queuell check-balgosrbkw-03-unionfind
)
