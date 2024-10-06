set (SOURCES
  base.h
  color.h
  message.h
  control.h
  command.h
  parameter.h
  toggle.h
  program.h
  executor.h
  cli-menu.h

  base.cpp
  color.cpp
  message.cpp
  control.cpp
  command.cpp
  parameter.cpp
  toggle.cpp
  program.cpp
  executor.cpp
)

add_library(cli-menu STATIC ${SOURCES})
target_link_libraries(cli-menu mini-tools)
