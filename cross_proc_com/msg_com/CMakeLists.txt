set(MSG_COM msg_com)

add_executable(${MSG_COM})

add_subdirectory(src)
target_include_directories(${MSG_COM} PRIVATE include)