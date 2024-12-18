cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(demon)

add_executable(${PROJECT_NAME})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(${PROJECT_NAME}
                        PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
