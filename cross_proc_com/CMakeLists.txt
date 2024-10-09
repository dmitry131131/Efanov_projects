cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(IPC)

add_subdirectory(FIFO_com)
add_subdirectory(msg_com)
add_subdirectory(shmem_com)