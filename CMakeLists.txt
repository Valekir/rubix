cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(rubix)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

file(GLOB SOURCE_FILES 
	src/controller.cpp
	src/cube.cpp
	src/menu.cpp
	src/test.cpp
	src/utils.cpp
	src/view.cpp
)

add_library(rubix_lib STATIC ${SOURCE_FILES})

target_include_directories(rubix_lib PUBLIC ${INCLUDE_DIR})

add_executable(rubix ${SOURCE_DIR}/main.cpp)

target_link_libraries(rubix rubix_lib)

find_package(Curses REQUIRED)
target_link_libraries(rubix ${CURSES_LIBRARIES})