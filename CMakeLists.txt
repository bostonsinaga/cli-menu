cmake_minimum_required(VERSION 3.10)
project(test)

set(minitools_dir submodules/mini-tools/includes)

add_subdirectory(${minitools_dir})
add_subdirectory(includes)
add_subdirectory(test)

target_include_directories(cli-menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${minitools_dir})
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
