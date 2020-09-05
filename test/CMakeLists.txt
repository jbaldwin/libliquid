cmake_minimum_required(VERSION 3.16)
project(libliquid_test)

set(LIBLIQUID_TEST_SOURCE_FILES
    main.cpp
    TestParseRequest.cpp
    TestParseResponse.cpp
    TestSimd.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${LIBLIQUID_TEST_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE liquid)

if(LIQUID_USE_SSE42)
    target_compile_options(${PROJECT_NAME} PRIVATE -msse4.2)
endif()

if(LIQUID_CODE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
    target_link_libraries(${PROJECT_NAME} PRIVATE gcov)
endif()

add_test(NAME liquid_test COMMAND ${PROJECT_NAME})
