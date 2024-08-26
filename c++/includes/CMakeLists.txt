set (SOURCES
  base.h
  command.h
  program.h
  parameter.h
  toggle.h
  message.h
  executor.h
  cli-menu.h

  base.cpp
  command.cpp
  program.cpp
  parameter.cpp
  toggle.cpp
  message.cpp
  executor.cpp
)

add_library(cli-menu STATIC ${SOURCES})
target_link_libraries(cli-menu mini-tools)
