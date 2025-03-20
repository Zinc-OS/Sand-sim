cmake_minimum_required(VERSION 3.10)

project(SandSim)
find_package(SDL2 REQUIRED)

include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})

add_executable(sandsim main.c color.c)

target_include_directories(sandsim AFTER PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sandsim ${SDL2_LIBRARIES})
