cmake_minimum_required(VERSION 3.10.2)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(astar-visual CXX)
include_directories(./include)
include_directories(./lib/SDL2-2.0.16/include)
link_directories(./lib/SDL2-2.0.16/lib/x64)

SET (SRC_DIR ./src)

file(GLOB SRC_FILES ${SRC_DIR}/*.cpp ./include/*.h)

add_executable(astar-visual ${SRC_FILES})
target_link_libraries(astar-visual Sdl2)