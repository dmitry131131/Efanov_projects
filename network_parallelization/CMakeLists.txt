cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(Network)

set(CLIENT Client)
set(SERVER Server)

add_executable(${CLIENT})
add_executable(${SERVER})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/client)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/server)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common)

set_target_properties(${CLIENT} ${SERVER}
                        PROPERTIES 
                        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)