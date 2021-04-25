cmake_minimum_required(VERSION 3.16)
project(turbohttp CXX)

option(TURBOHTTP_BUILD_EXAMPLES "Build the examples, Default=ON." ON)
option(TURBOHTTP_BUILD_TESTS    "Build the tests, Default=ON." ON)
option(TURBOHTTP_CODE_COVERAGE  "Enable code coverage, tests must also be enabled, Default=OFF" OFF)

if(NOT DEFINED TURBOHTTP_HEADER_COUNT)
    set(
        TURBOHTTP_HEADER_COUNT
        16
        CACHE STRING
        "Set the default number of HTTP request and response headers, default value=16."
    )
endif()

message("${PROJECT_NAME} TURBOHTTP_BUILD_EXAMPLES   = ${TURBOHTTP_BUILD_EXAMPLES}")
message("${PROJECT_NAME} TURBOHTTP_BUILD_TESTS      = ${TURBOHTTP_BUILD_TESTS}")
message("${PROJECT_NAME} TURBOHTTP_CODE_COVERAGE    = ${TURBOHTTP_CODE_COVERAGE}")
message("${PROJECT_NAME} TURBOHTTP_HEADER_COUNT     = ${TURBOHTTP_HEADER_COUNT}")

set(LIBTURBOHTTP_SOURCE_FILES
    src/turbohttp/method.hpp
    src/turbohttp/parser.hpp src/turbohttp/parser.tcc
    src/turbohttp/turbohttp.hpp
    src/turbohttp/version.hpp
)

add_library(${PROJECT_NAME} STATIC ${LIBTURBOHTTP_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${TURBOHTTP_HEADER_COUNT})

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

if(TURBOHTTP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(TURBOHTTP_BUILD_TESTS)
    if(TURBOHTTP_CODE_COVERAGE)
        target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
        target_link_libraries(${PROJECT_NAME} PRIVATE gcov)
    endif()

    enable_testing()
    add_subdirectory(test)
endif()