cmake_minimum_required(VERSION 3.16)
# Benchmark
project(libturbohttp_readme)
add_executable(${PROJECT_NAME} readme.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE turbohttp)