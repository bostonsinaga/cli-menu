cmake_minimum_required(VERSION 3.10)
project(geometrics)

set (SOURCES
  #headers
  point.h
  size.h
  rect.h
  polygon.h
  ray-casting.h
  geometrics.h

  #sources
  point.cpp
  rect.cpp
  polygon.cpp
  ray-casting.cpp
)

add_library(geometrics STATIC ${SOURCES})
target_include_directories(geometrics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
