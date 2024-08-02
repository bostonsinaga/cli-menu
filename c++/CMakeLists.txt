cmake_minimum_required(VERSION 3.10)
project(TEST)

set(minitools_dir submodules/mini-tools/c++/includes)

add_subdirectory(${minitools_dir})
add_subdirectory(includes)
add_subdirectory(test)

target_include_directories(TEST PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/${minitools_dir}
  ${CMAKE_CURRENT_SOURCE_DIR}/includes
)
