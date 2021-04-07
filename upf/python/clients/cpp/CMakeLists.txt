cmake_minimum_required(VERSION 3.4)
project(cpp_client)

add_subdirectory(pybind11)

add_executable(cpp_client main.cpp)
target_link_libraries(cpp_client PRIVATE pybind11::embed)
