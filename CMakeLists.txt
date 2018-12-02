cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)

project(libliquid)

set(PROJECT_LIBRARY_DEPS

)

set(SYSTEM_LIBRARY_DEPS

)

set(LIBLIQUID_SOURCE_FILES
    inc/liquid/request/Request.h src/request/Request.cpp

    inc/liquid/Liquid.h
    inc/liquid/Method.h src/Method.cpp
    inc/liquid/Version.h src/Version.cpp
)

add_library(${PROJECT_NAME} ${LIBLIQUID_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC inc)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${PROJECT_LIBRARY_DEPS}
    ${SYSTEM_LIBRARY_DEPS}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Weffc++
    -Werror
    -Wpedantic
    -pedantic-errors
)

add_subdirectory(test)