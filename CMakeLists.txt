cmake_minimum_required(VERSION 3.10)
project(Lexer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

add_executable(lexer lexer.cpp)