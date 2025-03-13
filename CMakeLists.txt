cmake_minimum_required(VERSION 3.10)

project(SandSim)

set(SDL2_DIR "/SDL2/x86_64-w64-mingw32/lib/cmake/SDL2")
find_package(SDL2 REQUIRED)
include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})

add_executable(sandsim main.c)

target_link_libraries(sandsim ${SDL2_LIBRARIES})
