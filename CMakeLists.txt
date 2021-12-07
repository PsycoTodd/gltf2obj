cmake_minimum_required (VERSION 3.5.1)

### C++11 standards
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX+EXTENSIONS OFF)


project(gltf2obj)

include_directories("draco/src")

add_executable(${PROJECT_NAME} gltf2obj/main.cpp)