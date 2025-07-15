set (SOURCES
  color.cpp
  console.cpp
  language.cpp
  result.cpp
  control.cpp
  clipboard.cpp
  command.cpp
  word.cpp
  number.cpp
  toggle.cpp
  program.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} mini-tools)
