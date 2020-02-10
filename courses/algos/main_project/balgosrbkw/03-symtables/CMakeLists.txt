add_subdirectory(bsttable)
add_subdirectory(lltable)

add_custom_target(check-balgosrbkw-03
   DEPENDS check-balgosrbkw-03-bsttable check-balgosrbkw-03-lltable
)
