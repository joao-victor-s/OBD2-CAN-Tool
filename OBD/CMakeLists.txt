cmake_minimum_required(VERSION 3.10)
project(OBD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(OBD
    src/CANManager.cpp
    src/OBDReader.cpp
    main.cpp
)

add_executable(simulator
    src/CANManager.cpp
    src/OBDReader.cpp
    simulator.cpp
)