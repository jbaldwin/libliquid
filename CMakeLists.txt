cmake_minimum_required(VERSION 3.16)
project(liquid CXX)

option(LIQUID_USE_SSE42     "Build with SSE4.2 instructions, Default=ON." ON)
option(LIQUID_BUILD_TESTS   "Build the tests, Default=ON." ON)
option(LIQUID_CODE_COVERAGE "Enable code coverage, tests must also be enabled, Default=OFF" OFF)

message("${PROJECT_NAME} LIQUID_USE_SSE42       = ${LIQUID_USE_SSE42}")
message("${PROJECT_NAME} LIQUID_BUILD_TESTS     = ${LIQUID_BUILD_TESTS}")
message("${PROJECT_NAME} LIQUID_CODE_COVERAGE   = ${LIQUID_CODE_COVERAGE}")

set(LIBLIQUID_SOURCE_FILES
    src/liquid/liquid.hpp
    src/liquid/method.hpp
    src/liquid/parser.hpp src/liquid/parser.cpp
    src/liquid/version.hpp
)

add_library(${PROJECT_NAME} STATIC ${LIBLIQUID_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC src)

if(LIQUID_USE_SSE42)
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

if(LIQUID_BUILD_TESTS)
    if(LIQUID_CODE_COVERAGE)
        target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
        target_link_libraries(${PROJECT_NAME} PRIVATE gcov)
    endif()

    enable_testing()
    add_subdirectory(test)
    add_subdirectory(perf)
endif()