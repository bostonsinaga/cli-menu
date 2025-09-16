add_executable(users main.cpp)

target_link_libraries(users PRIVATE
  mini-tools ${PROJECT_NAME}
)
