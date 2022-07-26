set(BINARY ${CMAKE_PROJECT_NAME}_test)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${BINARY} test_main.cpp ${TEST_SOURCES})

include(GoogleTest)
gtest_discover_tests(${BINARY})

target_link_libraries(${BINARY} PUBLIC
    gtest
    gmock
    pthread
)