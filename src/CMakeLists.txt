cmake_minimum_required(VERSION 3.10)

project(sandsim)
find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

include_directories(sandsim ${SDL2_INCLUDE_DIRS})
include_directories(sandsim ${SDL2IMAGE_INCLUDE_DIRS})

add_executable(sandsim main.c color.c letterbox.c)

target_include_directories(sandsim AFTER PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sandsim ${SDL2_LIBRARIES})
target_link_libraries(sandsim ${SDL2IMAGE_LIBRARIES})
