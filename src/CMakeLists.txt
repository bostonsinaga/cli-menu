set (SOURCES
  color.cpp
  console.cpp
  language.cpp
  result.cpp
  clipboard.cpp
  control.cpp
  command.cpp
  word.cpp
  number.cpp
  toggle.cpp
)

add_library(cli-menu STATIC ${SOURCES})
target_link_libraries(cli-menu mini-tools)
