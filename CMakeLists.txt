cmake_minimum_required(VERSION 3.10)

project(Lab1)

SET(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")

add_executable(lc 
    main.c
    parallel.c
    queue.c
    myerror.c
    global.c
)

target_include_directories(lc PUBLIC "${PROJECT_BINARY_DIR}")

TARGET_LINK_LIBRARIES( lc
    pthread
)