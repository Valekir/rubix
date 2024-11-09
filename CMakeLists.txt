cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(rubix)

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB cube.cpp view.cpp utils.cpp controller.cpp)

add_library(cube STATIC ${SOURCE_LIB})
add_library(view STATIC ${SOURCE_LIB})
add_library(utils STATIC ${SOURCE_LIB})
add_library(controller STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE_EXE})

target_link_libraries(main cube view utils controller)
