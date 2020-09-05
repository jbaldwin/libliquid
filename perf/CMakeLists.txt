cmake_minimum_required(VERSION 3.16)
project(libturbohttp_perf)

set(LIBTURBOHTTP_PERF_SOURCE_FILES
    main.cpp
)

add_executable(${PROJECT_NAME} ${LIBTURBOHTTP_PERF_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE turbohttp)

if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        -Wno-unknown-pragmas
    )
endif()
if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Weffc++
        -Werror
        -Wpedantic
        -pedantic-errors
    )
endif()

if(USE_SSE42)
    target_compile_options(${PROJECT_NAME} PRIVATE -msse4.2)
endif()
