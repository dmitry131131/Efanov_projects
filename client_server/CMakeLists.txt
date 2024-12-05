cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(Client_server)

set(CLIENT Client)
set(SERVER Server)

add_executable(${CLIENT})
add_executable(${SERVER})

add_subdirectory(client)
add_subdirectory(server)

set_target_properties(${CLIENT} ${SERVER}
                        PROPERTIES 
                        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)