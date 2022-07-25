cmake_minimum_required(VERSION 3.20)
project(lhs)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# include header files of lib
include_directories(src)
include_directories(include)

# link ddl in ./lib
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/gmock)

# main executable
add_subdirectory(src)
add_executable(lhs ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

# link env lib
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lhs PUBLIC Threads::Threads)

# link static lib
target_link_libraries(lhs PUBLIC container model server thread_pool)

# test configuration
enable_testing()
add_subdirectory(test)