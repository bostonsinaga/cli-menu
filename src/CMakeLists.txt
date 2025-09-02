set (SOURCES
  color.cpp
  console.cpp
  language.cpp
  control.cpp
  result.cpp
  clipboard.cpp
  command.cpp
  type.cpp
  preset.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} mini-tools)
