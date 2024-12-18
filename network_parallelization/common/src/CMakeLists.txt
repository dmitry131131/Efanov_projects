target_sources(${SERVER} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/buffer.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/integral.c)
target_sources(${CLIENT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/buffer.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/integral.c)
