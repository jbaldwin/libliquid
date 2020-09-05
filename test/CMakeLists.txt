cmake_minimum_required(VERSION 3.16)
project(libturbohttp_test)

set(LIBTURBOHTTP_TEST_SOURCE_FILES
    main.cpp
    TestParseRequest.cpp
    TestParseResponse.cpp
    TestSimd.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${LIBTURBOHTTP_TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE turbohttp)

if(TURBOHTTP_USE_SSE42)
    target_compile_options(${PROJECT_NAME} PRIVATE -msse4.2)
endif()

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
