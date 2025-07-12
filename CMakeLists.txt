cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(cli-menu VERSION 1.0.0)

message(STATUS "Project version: ${PROJECT_VERSION}")
message(STATUS "Major version: ${PROJECT_VERSION_MAJOR}")
message(STATUS "Minor version: ${PROJECT_VERSION_MINOR}")
message(STATUS "Patch version: ${PROJECT_VERSION_PATCH}")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mini-tools)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
