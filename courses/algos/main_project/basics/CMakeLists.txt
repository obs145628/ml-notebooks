add_subdirectory(calc)
add_subdirectory(cat)
add_subdirectory(fact)
add_subdirectory(fibo)
add_subdirectory(printer)


add_custom_target(check-basics
   DEPENDS check-basics-calc check-basics-cat check-basics-fact check-basics-fibo check-basics-printer
)
