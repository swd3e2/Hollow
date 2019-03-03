cmake_minimum_required(VERSION 3.14)

project(Hollow Engine)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDART 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -WALL -02")

file(GLOB CPPS "*.cpp")

add_executable(${PROJECT_NAME} ${CPPS})