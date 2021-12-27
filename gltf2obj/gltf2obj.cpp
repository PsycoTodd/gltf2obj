#include "gltf2obj.h"
#include "tiny_gltf.h"
#include "simpleLog.h"

#include <sstream>


namespace gaic
{

int gltf2obj::loadGLTFGeometry(const tinygltf::Model& model, 
                               vector<Vertex>& meshData, 
                               vector<size_t>& indexData)
{
  // load the scene first.
  int total_vert = 0;
  const tinygltf::Scene &gltfScene = model.scenes[model.defaultScene];
  for(const tinygltf::Mesh &gltfMesh : model.meshes)
  {
    if(gltfMesh.primitives.empty()) {
      log(log_level::Info, "No primitive to parse.");
      return 0;
    }
    if(gltfMesh.primitives.size() > 1) {
      log(log_level::Info, "we only convert obj generated gltf, which may contains only single primitive. You only get the first one.");
    }

    for(size_t i = 0; i < gltfMesh.primitives.size(); ++i) {
      const tinygltf::Primitive &primitive = gltfMesh.primitives[i];
      if(primitive.indices < 0){
        log(log_level::Warning, "mesh is empty of indices, loading fails.");
        return 0;
      }

      std::map<std::string, int>::const_iterator it(primitive.attributes.begin());
      std::map<std::string, int>::const_iterator itEnd(primitive.attributes.end());

      bool containNormals = false;
      size_t num_vert = 0;
      size_t size = 1;
      bool firstTimeParse = true; // since accessor are all together, we need to get the total vert number to set the buffer only once.
      for(; it != itEnd; ++it) {
        if (it->second < 0) continue;
        const tinygltf::Accessor &accessor = model.accessors[it->second];
        const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

        if (firstTimeParse)
        {
          num_vert = accessor.count;
          meshData.resize(num_vert);
          total_vert += num_vert;
          firstTimeParse = false;
        }

        switch(accessor.type){
          case TINYGLTF_TYPE_SCALAR:
            size = 1;
            break;
          case TINYGLTF_TYPE_VEC2:
            size = 2;
            break;
          case TINYGLTF_TYPE_VEC3:
            size = 3;
            break;
          case TINYGLTF_TYPE_VEC4:
            size = 4;
            break;
          default:
            log(log_level::Error, "No support accessor, load fails.");
            return 0;
        }

        int stride = bufferView.byteStride > 0 ? bufferView.byteStride : size * sizeof(float);
        
        if (!it->first.compare("POSITION")) {
          for (auto k = 0; k < num_vert; ++k) {
            const void * pData = &(buffer.data.at(0)) + bufferView.byteOffset + accessor.byteOffset  + k * stride;
            std::memcpy(meshData[k].position, pData, stride);
          }
        }
        else if (!it->first.compare("NORMAL")) {
          for (auto k = 0; k < num_vert; ++k) {
            const void * pData = &(buffer.data.at(0)) + bufferView.byteOffset + accessor.byteOffset  + k * stride;
            std::memcpy(meshData[k].normal, pData, stride);
          }
          containNormals = true;
        }
        else if (!it->first.compare("TEXCOORD_0")) {
          for (auto k = 0; k < num_vert; ++k) {
            const void * pData = &(buffer.data.at(0)) + bufferView.byteOffset + accessor.byteOffset  + k * stride;
            std::memcpy(meshData[k].texCoord, pData, stride);
          }
        }
        else {
          log(log_level::Error, "data type is not supported.");
        }
      }
      // Try to get the data from the index buffer.
      const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];
      size_t numIndices = indexAccessor.count;
      indexData.resize(numIndices);
      const tinygltf::BufferView &indexBufferView = model.bufferViews[indexAccessor.bufferView];
      const tinygltf::Buffer &indexBuffer = model.buffers[model.bufferViews[indexAccessor.bufferView].buffer];
      switch(indexAccessor.componentType) {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
          size = sizeof(uint8_t);
          break;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
          size = sizeof(unsigned short);
          break;
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
          size = sizeof(unsigned int);
          break;
        default:
          log(log_level::Warning, "index type is not supported.");
          return 0;
      }

      for (auto k = 0; k < numIndices; ++k) {
        const void *pData = &(indexBuffer.data.at(0)) + indexBufferView.byteOffset + indexAccessor.byteOffset + k * size;
        std::memcpy(&(indexData[k]), pData, size);
      }

    }

    return total_vert;
  }
}

int gltf2obj::loadGLTFMaterial(const tinygltf::Model& model, vector<Material>& materialData)
{
  return 0;
}

string gltf2obj::generateObjFromMeshData(const vector<Vertex>& meshData, 
                                         const vector<size_t>& indexData, 
                                         const std::string& prefixString,
                                         const std::string& fileNameKey)
{
  // based on the obj definition, we can only get all vertex smeshed into one continus list.
  std::ostringstream objStr;
  char buffer[125];
  // build the head part.
  objStr << prefixString << "#" << std::endl
         << "mtllib " << fileNameKey << ".mtl" << std::endl
         << "# Object0 follows..." << std::endl;

  // Write the vertices
  for(const auto& v : meshData) 
  {
    sprintf(buffer, "v %.6f %.6f %.6f\n", v.position[0], v.position[1], v.position[2]);
    objStr << buffer;
  }
  objStr << "# " << meshData.size() << " vertices" << std::endl << std::endl;

  // Write the texcoords
  for(const auto& v : meshData) 
  {
    sprintf(buffer, "vt %.6f %.6f\n", v.texCoord[0], 1.0 - v.texCoord[1]);
    objStr << buffer;
  }
  objStr << "# " << meshData.size() << " texture vertices" << std::endl << std::endl;

  // Write the vertex normals
  for(const auto& v : meshData) 
  {
    sprintf(buffer, "vn %.6f %.6f %.6f\n", v.normal[0], v.normal[1], v.normal[2]);
    objStr << buffer;
  }
  objStr << "# " << meshData.size() << " normals" << std::endl << std::endl;
  
  // Now give the triangle info
  objStr << "g " << fileNameKey << std::endl
         << "usemtl " << fileNameKey << std::endl;
  
  for(int i = 0; i < indexData.size(); i += 3)
  {
    size_t i1 = indexData[i+0] + 1, i2 = indexData[i+1] + 1, i3 = indexData[i+2] + 1;
    sprintf(buffer, "f %lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu\n", i1, i1, i1, i2, i2, i2, i3, i3, i3);
    objStr << buffer;
  }
  objStr << "# " << indexData.size() / 3 << " triangles" << std::endl;

  return objStr.str();
}

}

