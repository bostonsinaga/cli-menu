set (SOURCES
  color.cpp
  console.cpp
  language.cpp
  control.cpp
  data.cpp
  clipboard.cpp
  command.cpp
  type.cpp
  preset.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE mini-tools)
