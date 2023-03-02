cmake_minimum_required(VERSION 3.24)
project(key)

set(CMAKE_CXX_STANDARD 17)

add_executable(key main.cpp)
target_link_libraries(key Wininet  )
