#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_ENABLE_DRACO
#include "tiny_gltf.h"
#include <iostream>
#include <string>
#include "gltf2obj.h"

bool dummyLoadImageDataFunction(tinygltf::Image *, const int, std::string *,
                                std::string *, int, int,
                                const unsigned char *, int,
                                void *user_pointer)
{
  return true;
}

int main(int argc, char *argv[])
{
  if(argc < 2) {
    std::cout<<"Please provide the input gltf file path."<<std::endl;
    return -1;
  }

  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err, warn;

  loader.SetImageLoader(dummyLoadImageDataFunction, nullptr);
  bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, argv[1]);

  using namespace gaic;
  vector<Vertex> meshdata;
  gltf2obj::loadGLTFGeometry(model, meshdata);

  if(ret) {
    std::cout<<model.extensionsUsed[0]<<std::endl;
  }
  else {
    std::cout<<err<<std::endl;
  }
  
}