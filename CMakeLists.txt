cmake_minimum_required(VERSION 3.14)

project(name C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE files "src/*.c")

add_executable(out ${files})

target_compile_options(out PRIVATE "-g3" "-mavx2" "-fsanitize=address" "-std=gnu23" "--embed-dir=.")
target_link_libraries(out PRIVATE "asan" "GL" "glfw" "GLEW" "GLU" "m")

target_include_directories(out PRIVATE "include")
