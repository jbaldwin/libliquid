cmake_minimum_required(VERSION 3.11)
project(libliquid_perf)

set(LIBLIQUID_PERF_SOURCE_FILES
    main.cpp
)

add_executable(${PROJECT_NAME} ${LIBLIQUID_PERF_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE libliquid)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Weffc++
    -Werror
    -Wpedantic
    -pedantic-errors
)

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
        -O2
        -g
        -gdwarf-2
    )
endif()
