#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_ENABLE_DRACO
#include "tiny_gltf.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  if(argc < 2) {
    std::cout<<"Please provide the input gltf file path."<<std::endl;
    return -1;
  }

  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err, warn;

  bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, argv[1])

  if(ret) {
    std::cout<<model.extensionsUsed[0]<<std::endl;
  }
  
}