cmake_minimum_required (VERSION 3.5.1)

### C++11 standards
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX+EXTENSIONS OFF)


project(gltf2obj)
include_directories(${PROJECT_SOURCE_DIR})
include_directories("draco/build_dir")
include_directories("draco/src/")
link_directories("draco/build_dir")


add_executable(${PROJECT_NAME} gltf2obj/main.cpp gltf2obj/gltf2obj.cpp)
target_link_libraries(${PROJECT_NAME} libdraco.a)