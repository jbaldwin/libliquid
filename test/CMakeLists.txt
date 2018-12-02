cmake_minimum_required(VERSION 3.11)
project(libliquid_test)

set(LIBLIQUID_TEST_SOURCE_FILES
    Catch2.h
    main.cpp
    TestParseRequest.cpp
)

add_executable(libliquid_test ${LIBLIQUID_TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE libliquid)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Weffc++
    -Werror
    -Wpedantic
    -pedantic-errors
)
