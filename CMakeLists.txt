cmake_minimum_required (VERSION 3.5.1)

### C++11 standards
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX+EXTENSIONS OFF)


project(gltf2obj)

include_directories(${PROJECT_SOURCE_DIR})
include_directories("draco/build_dir")
include_directories("draco/src/")
if(WIN32)
  link_directories("draco/build_dir/${CMAKE_BUILD_TYPE}")
else()
  link_directories("draco/build_dir")
endif()


add_executable(${PROJECT_NAME} gltf2obj/main.cpp gltf2obj/gltf2obj.cpp gltf2obj/simpleLog.cpp)
if(WIN32)
  target_link_libraries(${PROJECT_NAME} draco)
else()
  target_link_libraries(${PROJECT_NAME} libdraco.a)
endif()