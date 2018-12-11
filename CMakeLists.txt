cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)

project(liquid)

option(USE_SSE42 "Build with SSE4.2 instructions." ON)

set(PROJECT_LIBRARY_DEPS

)

set(SYSTEM_LIBRARY_DEPS

)

set(LIBLIQUID_SOURCE_FILES
    inc/liquid/Liquid.h
    inc/liquid/Method.h src/Method.cpp
    inc/liquid/Parser.h inc/liquid/Parser.tcc src/Parser.cpp
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

if(USE_SSE42)
    target_compile_options(${PROJECT_NAME} PRIVATE -msse4.2)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message("${PROJECT_NAME} is building Debug")
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        -O0
        -g
        -gdwarf-2
    )
    if(D_GLIBCXX_DEBUG)
        message("${PROJECT_NAME} D_GLIBCXX_DEBUG is ON")
        target_compile_options(${PROJECT_NAME} PRIVATE -D_GLIBCXX_DEBUG)
    else()
        message("${PROJECT_NAME} D_GLIBCXX_DEBUG is OFF")
    endif()

else() # "Release"
    message("${PROJECT_NAME} is building Release")
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        -O3
        -g
        -gdwarf-2
    )
endif()

add_subdirectory(test)
add_subdirectory(perf)