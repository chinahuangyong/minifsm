cmake_minimum_required(VERSION 3.10)
project(StateMachine)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(fsm src/fsm.cpp)

add_executable(StateMachine src/main.cpp)
target_link_libraries(StateMachine
        fsm
    pthread)
