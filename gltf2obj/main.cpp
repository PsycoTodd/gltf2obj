#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_ENABLE_DRACO
#include "tiny_gltf.h"
#include <string>
#include <fstream>
#include <iostream>
#include "gltf2obj.h"
#include "simpleLog.h"

using namespace gaic;

// Helper function to get directory from path
std::string getDirectory(const std::string& filepath) {
  size_t pos = filepath.find_last_of("/\\");
  if(pos != std::string::npos) {
    return filepath.substr(0, pos);
  }
  return "";
}

// Helper function to get filename from path
std::string getFilename(const std::string& filepath) {
  size_t pos = filepath.find_last_of("/\\");
  if(pos != std::string::npos) {
    return filepath.substr(pos + 1);
  }
  return filepath;
}

// Helper function to get filename without extension
std::string getFilenameWithoutExtension(const std::string& filepath) {
  std::string filename = getFilename(filepath);
  size_t pos = filename.find_last_of('.');
  if(pos != std::string::npos) {
    return filename.substr(0, pos);
  }
  return filename;
}

// Helper function to copy file
bool copyFile(const std::string& source, const std::string& dest) {
  std::ifstream src(source, std::ios::binary);
  if(!src.is_open()) {
    return false;
  }

  std::ofstream dst(dest, std::ios::binary);
  if(!dst.is_open()) {
    src.close();
    return false;
  }

  dst << src.rdbuf();

  src.close();
  dst.close();
  return true;
}

bool dummyLoadImageDataFunction(tinygltf::Image *, const int, std::string *,
                                std::string *, int, int,
                                const unsigned char *, int,
                                void *user_pointer)
{
  return true;
}

// Function to create MTL file and copy texture
void createObjMtl(const Material& material, const std::string& inputGltfPath, const std::string& mtlPath)
{
  // Get the directory of the input GLTF file
  std::string inputDir = getDirectory(inputGltfPath);

  // Get the directory of the output MTL file
  std::string outputDir = getDirectory(mtlPath);

  // Create MTL file
  std::ofstream mtlFile(mtlPath);
  if(!mtlFile.is_open()) {
    log(log_level::Error, "Failed to create MTL file: " + mtlPath);
    return;
  }

  // Extract material name from mtl path (without extension)
  std::string materialName = getFilenameWithoutExtension(mtlPath);

  // Write MTL content
  mtlFile << "newmtl " << materialName << std::endl;
  mtlFile << "Kd " << material.diffuseColor[0] << " "
          << material.diffuseColor[1] << " "
          << material.diffuseColor[2] << std::endl;

  // Handle texture if it exists
  if(!material.diffuseTexture.empty()) {
    std::string textureFilename = getFilename(material.diffuseTexture);

    // Source texture path (relative to input GLTF)
    std::string sourceTexture;
    if(inputDir.empty()) {
      sourceTexture = material.diffuseTexture;
    } else {
      sourceTexture = inputDir + "/" + material.diffuseTexture;
    }

    // Destination texture path (same directory as MTL file)
    std::string destTexture;
    if(outputDir.empty()) {
      destTexture = textureFilename;
    } else {
      destTexture = outputDir + "/" + textureFilename;
    }

    // Copy the texture file
    if(copyFile(sourceTexture, destTexture)) {
      log(log_level::Info, "Copied texture: " + sourceTexture + " to " + destTexture);
    } else {
      log(log_level::Warning, "Failed to copy texture file: " + sourceTexture);
    }

    // Write texture reference in MTL file
    mtlFile << "map_Kd " << textureFilename << std::endl;
  }

  mtlFile.close();
  log(log_level::Info, "Created MTL file: " + mtlPath);
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

  // work on diffuse texture if needed.
  if (createTextureFlag) {
    using namespace gaic;
    vector<Material> materialdata;
    gltf2obj::loadGLTFMaterial(model, materialdata);
    // right now just output first one.
    if (!materialdata.empty()) {
      std::string mtlPath = outputPath.substr(0, outputPath.rfind('.')) + ".mtl";
      createObjMtl(materialdata[0], argv[1], mtlPath);
    }
  }

  return 0;
}