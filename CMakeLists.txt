cmake_minimum_required(VERSION 3.20)
project(lhs_Sample)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_library(lhs SHARED
        src/container/HttpHandlerMap.cpp
        src/container/HttpHandlerMap.h
        src/container/SafeQueue.h
        src/model/HttpException.cpp
        src/model/HttpException.h
        src/model/HttpRequest.cpp
        src/model/HttpRequest.h
        src/model/HttpResponse.cpp
        src/model/HttpResponse.h
        src/threadPool/ThreadPool.cpp
        src/threadPool/ThreadPool.h
        src/HttpServer.cpp
        src/HttpServer.h)

set_target_properties(lhs PROPERTIES VERSION 1.0.0)

add_executable(lhs_test main.cpp)

target_link_libraries(lhs_test lhs)

