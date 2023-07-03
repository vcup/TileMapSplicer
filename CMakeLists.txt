cmake_minimum_required(VERSION 3.24)
project(TileMapSplicer C)

include(${CMAKE_CURRENT_LIST_DIR}/build/conan_toolchain.cmake)

set(CMAKE_C_STANDARD 23)

find_package(libvips REQUIRED)
find_package(argtable3 REQUIRED)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} libvips::libvips)
target_link_libraries(${PROJECT_NAME} argtable3::argtable3)
