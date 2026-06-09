add_executable(log main.cpp)

target_link_libraries(log PRIVATE
  mini-tools ${PROJECT_NAME}
)
