project(cli-menu)

set (SOURCES
  # HEADERS
  cli-menu.h

  # SOURCES
  util.cpp
  command.cpp
  message.cpp
  executor.cpp
)

add_library(cli-menu STATIC ${SOURCES})
target_link_libraries(cli-menu mini-tools)
