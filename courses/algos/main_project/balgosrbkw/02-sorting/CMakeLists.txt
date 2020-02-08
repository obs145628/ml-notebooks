add_subdirectory(bumergesort)
add_subdirectory(insertionsort)
add_subdirectory(quicksort)
add_subdirectory(selectionsort)
add_subdirectory(shellsort)
add_subdirectory(tdmergesort)

add_custom_target(check-balgosrbkw-02
   DEPENDS check-balgosrbkw-02-bumergesort check-balgosrbkw-02-insertionsort check-balgosrbkw-02-quicksort check-balgosrbkw-02-selectionsort check-balgosrbkw-02-shellsort check-balgosrbkw-02-tdmergesort
)
