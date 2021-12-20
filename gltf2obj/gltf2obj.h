#ifndef GAIC_GLTF2OBJ
#define GAIC_GLTF2OBJ

#include <vector>

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
  float texCoord[3];
};

struct Material
{
  float diffuse[3];
};

class gltf2obj
{
public:
  static int loadGLTFGeometry(const tinygltf::Model& model, vector<Vertex>& meshData);
  static int loadGLTFMaterial(const tinygltf::Model& model, vector<Material>& materialData);
};

}

#endif