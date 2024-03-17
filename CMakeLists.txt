cmake_minimum_required(VERSION 3.28)
project(labwork8)

set(CMAKE_CXX_STANDARD 17)

add_executable(labwork8 main.cpp)

add_subdirectory(lib)
target_link_libraries(labwork8 PUBLIC BST)

enable_testing()
add_subdirectory(tests)
