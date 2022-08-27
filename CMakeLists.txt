cmake_minimum_required(VERSION 3.23)
project(FinalProject22 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O2 -g3 -fsanitize=address,undefined -static-libsan")

add_executable(FinalProject22 main.c)
