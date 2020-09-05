cmake_minimum_required(VERSION 3.16)
project(turbohttp CXX)

option(TURBOHTTP_USE_SSE42     "Build with SSE4.2 instructions, Default=ON." ON)
option(TURBOHTTP_BUILD_TESTS   "Build the tests, Default=ON." ON)
option(TURBOHTTP_CODE_COVERAGE "Enable code coverage, tests must also be enabled, Default=OFF" OFF)

message("${PROJECT_NAME} TURBOHTTP_USE_SSE42       = ${TURBOHTTP_USE_SSE42}")
message("${PROJECT_NAME} TURBOHTTP_BUILD_TESTS     = ${TURBOHTTP_BUILD_TESTS}")
message("${PROJECT_NAME} TURBOHTTP_CODE_COVERAGE   = ${TURBOHTTP_CODE_COVERAGE}")

set(LIBTURBOHTTP_SOURCE_FILES
    src/turbohttp/method.hpp
    src/turbohttp/parser.hpp src/turbohttp/parser.tcc
    src/turbohttp/turbohttp.hpp
    src/turbohttp/version.hpp
)

add_library(${PROJECT_NAME} STATIC ${LIBTURBOHTTP_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC src)

if(TURBOHTTP_USE_SSE42)
    target_compile_options(${PROJECT_NAME} PUBLIC -msse4.2)
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

if(TURBOHTTP_BUILD_TESTS)
    if(TURBOHTTP_CODE_COVERAGE)
        target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
        target_link_libraries(${PROJECT_NAME} PRIVATE gcov)
    endif()

    enable_testing()
    add_subdirectory(test)
    add_subdirectory(perf)
endif()