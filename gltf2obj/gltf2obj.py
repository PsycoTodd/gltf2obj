import argparse
from os import access
from os import path
import struct
from pygltflib import GLTF2
import DracoPy

def gltf2obj(gltf, dirPath):
    # first, get mesh, normal, and UV and output obj
    nodesCount = len(gltf.scenes[gltf.scene].nodes)
    for i in range(nodesCount):
        mesh = gltf.meshes[gltf.scenes[gltf.scene].nodes[i]]
        for primitive in mesh.primitives:
            accessor = gltf.accessors[primitive.attributes.POSITION]
            bufferViewIdx = primitive.extensions["KHR_draco_mesh_compression"]["bufferView"]
            #bufferViewIdx = accessor.bufferView
            bufferView = gltf.bufferViews[bufferViewIdx]
            buffer = gltf.buffers[bufferView.buffer]
            bufferFile = path.join(dirPath, buffer.uri)
            with open(bufferFile, 'rb') as draco_file:
                file_content = draco_file.read()
                mesh_object = DracoPy.decode_buffer_to_mesh(file_content)
            for ele in mesh_object.points:
                print(ele)
            #data = gltf.decode_data_uri(buffer.uri)
            # pull the vertex data
            #vertices = []
            #triangleVertCount = 3
            #for j in range(accessor.count):
            #    index = bufferView.byteOffset + accessor.byteOffset + j * triangleVertCount * 4
            #    d = data[index : index + triangleVertCount * 4]
            #    v = struct.unpack("<fff", d) # convert from base64 to three floats
            #    vertices.append(v)
            #    print(j, v)

    

def gltf2objFromFile(filePath):
    data = GLTF2().load(filePath)
    dirPath = path.dirname(filePath)
    gltf2obj(data, dirPath)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Please provide the path to the gltf file.")
    parser.add_argument('-f', '--file_path', help="the file path to the gltf file, which is converted from obj originally.")
    args = parser.parse_args()
    gltf2objFromFile(args.file_path)
