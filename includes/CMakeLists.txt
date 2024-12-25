set (SOURCES
  dash-test.h
  color.h
  message.h
  control.h
  result-inputs.h
  command.h
  parameter.h
  toggle.h
  program.h
  cli-menu.h

  color.cpp
  message.cpp
  control.cpp
  command.cpp
  parameter.cpp
  toggle.cpp
  program.cpp
)

add_library(cli-menu STATIC ${SOURCES})
target_link_libraries(cli-menu mini-tools)
