add_executable(organism main.cpp)

target_link_libraries(organism PRIVATE
  mini-tools ${PROJECT_NAME}
)
