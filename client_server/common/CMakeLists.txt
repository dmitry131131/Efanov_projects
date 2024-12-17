target_include_directories(${SERVER} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${CLIENT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)