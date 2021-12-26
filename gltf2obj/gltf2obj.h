#ifndef GAIC_GLTF2OBJ
#define GAIC_GLTF2OBJ

#include <vector>
#include <string>

namespace tinygltf
{
class Model;
}

using namespace std;

namespace gaic
{

struct Vertex
{
  float position[3];
  float normal[3];
  float texCoord[2];
};

struct Material
{
  float diffuse[3];
};

class gltf2obj
{
public:
  static int loadGLTFGeometry(const tinygltf::Model& model, vector<Vertex>& meshData, vector<size_t>& indexData);
  static int loadGLTFMaterial(const tinygltf::Model& model, vector<Material>& materialData);
  static string generateObjFromMeshData(const vector<Vertex>& meshData, const vector<size_t>& indexData, const std::string& fileNameKey);
};

}

#endif