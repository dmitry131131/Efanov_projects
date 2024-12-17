cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(demon)

add_executable(${PROJECT_NAME})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)