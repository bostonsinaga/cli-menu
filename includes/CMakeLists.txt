set (SOURCES
  base.cpp
  language.cpp
  clipboard.cpp
  color.cpp
  message.cpp
  control.cpp
  result-inputs.cpp
  command.cpp
  parameter.cpp
  toggle.cpp
  program.cpp
)

add_library(cli-menu STATIC ${SOURCES})
target_link_libraries(cli-menu mini-tools)
