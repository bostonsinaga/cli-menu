cmake_minimum_required(VERSION 3.10)
project(test VERSION 1.0.0)

message(STATUS "Project version: ${test_VERSION}")
message(STATUS "Major version: ${test_VERSION_MAJOR}")
message(STATUS "Minor version: ${test_VERSION_MINOR}")
message(STATUS "Patch version: ${test_VERSION_PATCH}")

set(minitools_dir mini-tools/includes)

add_subdirectory(${minitools_dir})
add_subdirectory(includes)
add_subdirectory(test)

target_include_directories(cli-menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${minitools_dir})
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

