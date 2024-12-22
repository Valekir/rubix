cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(rubix)

set(SOURCE_EXE main.cpp)
set(SOURCE_LIB cube.cpp view.cpp utils.cpp controller.cpp menu.cpp)

add_library(rubix_lib STATIC ${SOURCE_LIB})

find_package(Curses REQUIRED)

add_executable(main ${SOURCE_EXE})

target_link_libraries(main rubix_lib ${CURSES_LIBRARIES})
