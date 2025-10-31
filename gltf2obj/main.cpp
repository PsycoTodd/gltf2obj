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
  if(argc < 3) {
    std::cout<<"Please provide the input/output gltf file path."<<std::endl;
    return -1;
  }

  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err, warn;

  // Get the output file path and key
  std::string outputPath(argv[2]);

  // Get optional 3rd argument (integer)
  bool createTextureFlag = false;
  if(argc >= 4) {
    createTextureFlag = std::atoi(argv[3]) != 0;
  }

  std::cout<<createTextureFlag<<endl;

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

  // create the necessary prefix string
  std::ostringstream objStrStream;
  objStrStream <<"# converted by gltf2obj"<<std::endl
               << "# Produced by Dimensional Imaging OBJ exporter"<<std::endl
               << "# http://www.di3d.com" <<std::endl
               << "#" << std::endl
               << "# units mm" << std::endl;

  std::string objStr = gltf2obj::generateObjFromMeshData(meshdata, indexdata, objStrStream.str(), fileKey);

  if(!objStr.empty())
  {
    ofstream out(outputPath);
    out << objStr;
    out.close();
  }

  return 0;
}