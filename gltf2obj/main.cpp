#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_ENABLE_DRACO
#include "tiny_gltf.h"
#include <string>
#include <fstream>
#include "gltf2obj.h"
#include "simpleLog.h"

using namespace gaic;

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
    std::cout<<"Please provide the input/output gltf file path."<<std::endl;
    return -1;
  }

  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err, warn;

  // Get the output file path and key
  std::string outputPath(argv[2]);

  std::string filename = outputPath.substr(outputPath.find_last_of("/\\") + 1);
  std::string fileKey = filename.substr(0, filename.find('.'));

  loader.SetImageLoader(dummyLoadImageDataFunction, nullptr);
  bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, argv[1]);

  if(!err.empty()) {
    log(log_level::Error, err);
    return -1;
  }

  using namespace gaic;
  vector<Vertex> meshdata;
  vector<size_t> indexdata;
  gltf2obj::loadGLTFGeometry(model, meshdata, indexdata);
  std::string objStr = gltf2obj::generateObjFromMeshData(meshdata, indexdata, fileKey);

  if(!objStr.empty())
  {
    ofstream out(outputPath);
    out << objStr;
    out.close();
  }

  return 0;
}