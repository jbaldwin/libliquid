cmake_minimum_required(VERSION 3.16)

# Unit tests
project(libturbohttp_test)

set(LIBTURBOHTTP_TEST_SOURCE_FILES
    test_parse_request.cpp
    test_parse_response.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${LIBTURBOHTTP_TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE turbohttp)

if(TURBOHTTP_CODE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
    target_link_libraries(${PROJECT_NAME} PRIVATE gcov)
endif()

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

add_test(NAME turbohttp_test COMMAND ${PROJECT_NAME})

# Benchmark
project(libturbohttp_bench)

set(LIBTURBOHTTP_PERF_SOURCE_FILES
    bench.cpp
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

add_test(NAME turbohttp_bench COMMAND ${PROJECT_NAME})
