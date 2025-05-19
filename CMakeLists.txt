cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(cli-menu VERSION 1.0.0)

message(STATUS "Project version: ${cli_menu_VERSION}")
message(STATUS "Major version: ${cli_menu_VERSION_MAJOR}")
message(STATUS "Minor version: ${cli_menu_VERSION_MINOR}")
message(STATUS "Patch version: ${cli_menu_VERSION_PATCH}")

add_subdirectory(mini-tools/includes)
add_subdirectory(src)
add_subdirectory(test)

target_include_directories(cli-menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mini-tools/includes)
target_include_directories(cli-menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_include_directories(cli-menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
