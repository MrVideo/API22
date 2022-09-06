cmake_minimum_required(VERSION 3.23)
project(FinalProject22 C)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_DEBUG -Wall -Werror -g3 -fsanitize=address,undefined -static-libsan -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer")
set(CMAKE_OSX_ARCHITECTURES "arm64")

add_executable(FinalProject22 main.c)
