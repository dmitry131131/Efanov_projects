set(FIFO_COM FIFO_com)

add_executable(${FIFO_COM})

add_subdirectory(src)
target_include_directories(${FIFO_COM} PRIVATE include)