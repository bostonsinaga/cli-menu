set (SOURCES
  base.cpp
  color.cpp
  language.cpp
  message.cpp
  clipboard.cpp
  control.cpp
  result-inputs.cpp
  command.cpp
  parameter.cpp
  toggle.cpp
  program.cpp
)

add_library(cli_menu STATIC ${SOURCES})
target_link_libraries(cli_menu mini-tools)
