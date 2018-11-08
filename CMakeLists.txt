cmake_minimum_required (VERSION 2.6)
project (Euler)
add_executable(bin/euler src/euler.c)
target_link_libraries(bin/euler m)
