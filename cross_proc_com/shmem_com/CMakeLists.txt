set(SHMEM_COM shmem_com)

add_executable(${SHMEM_COM})

add_subdirectory(src)
target_include_directories(${SHMEM_COM} PRIVATE include)