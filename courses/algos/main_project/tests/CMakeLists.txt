add_custom_target(build-tests)

add_custom_target(check
  COMMAND
  OBTS_OUTPUT_DIR=${CMAKE_BINARY_DIR}/obts
  OBTS_BUILD_DIR=${CMAKE_BINARY_DIR}
  OBTS_BINARY_DIR=${CMAKE_BINARY_DIR}/bin
  python3 /home/obs/lun/obts/apps/tsauto.py ${CMAKE_SOURCE_DIR}
  DEPENDS build-tests
)
