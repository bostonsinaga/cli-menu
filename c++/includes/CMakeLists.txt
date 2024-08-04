set (SOURCES
  command.cpp
  message.cpp
  executor.cpp
)

add_library(cli-menu STATIC ${SOURCES})
target_link_libraries(cli-menu mini-tools)
