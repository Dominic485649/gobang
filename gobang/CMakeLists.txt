cmake_minimum_required(VERSION 4.0.2)
project(gobang)

set(CMAKE_C_STANDARD 23)

# 添加可执行文件
add_executable(gobang
    gobang.c
    chess_game.c
    machine.c
)