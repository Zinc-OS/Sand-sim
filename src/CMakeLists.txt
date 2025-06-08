cmake_minimum_required(VERSION 3.10)

project(sandsim)
find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog-extended)

include_directories(sandsim SDL2::SDL2 SDL2_image::SDL2_image)

add_executable(sandsim main.c color.c letterbox.c)

target_include_directories(sandsim AFTER PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sandsim SDL2::SDL2 SDL2_image::SDL2_image nfd) 


configure_file(air_button.png ${CMAKE_CURRENT_BINARY_DIR}/air_button.png COPYONLY)
configure_file(rock_button.png ${CMAKE_CURRENT_BINARY_DIR}/rock_button.png COPYONLY)
configure_file(image_button.png ${CMAKE_CURRENT_BINARY_DIR}/image_button.png COPYONLY)
configure_file(sand_button.png ${CMAKE_CURRENT_BINARY_DIR}/sand_button.png COPYONLY)
configure_file(load_image_button.png ${CMAKE_CURRENT_BINARY_DIR}/load_image_button.png COPYONLY)

