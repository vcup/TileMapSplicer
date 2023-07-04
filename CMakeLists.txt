cmake_minimum_required(VERSION 3.24)
project(TileMapSplicer C)

include(${CMAKE_CURRENT_LIST_DIR}/build/conan_toolchain.cmake)

set(CMAKE_C_STANDARD 23)

find_package(libvips REQUIRED)
find_package(argtable3 REQUIRED)

set(SRC_DIR "src/")
file(STRINGS srcfile.list SRCFILELIST)
list(TRANSFORM SRCFILELIST PREPEND ${SRC_DIR})
add_executable(${PROJECT_NAME} ${SRCFILELIST})

target_link_libraries(${PROJECT_NAME} libvips::libvips)
target_link_libraries(${PROJECT_NAME} argtable3::argtable3)
