import argparse
import os
import subprocess

# Make sure you have gltf2obj nodejs tool and gltf-pipeline installed.

def batchConvert(inputObjDir, outputGltfDir, doDracoCompression):
    for file in os.listdir(inputObjDir):
        if file.endswith(".obj"):
            outputFileName = ''
            separationFlag = ''
            if doDracoCompression:
                outputFileName = file.replace('.obj', '_raw.gltf')
            else:
                outputFileName = file.replace('.obj', '.gltf')
                separationFlag = '-st'
            args1 = ( 'obj2gltf', '-i', os.path.join(inputObjDir, file), '-o', os.path.join(outputGltfDir, outputFileName), separationFlag )
            os.system(' '.join(args1))
            if doDracoCompression:
                separationFlag = '-st'
                compressedOutputFileName = file.replace('.obj', '.gltf')
                args2 = ('gltf-pipeline', '-i', os.path.join(outputGltfDir, outputFileName), '-o', os.path.join(outputGltfDir, compressedOutputFileName), '-d', separationFlag)
                os.system(' '.join(args2))
                os.remove(os.path.join(outputGltfDir, outputFileName))
            print("converted " + file)

if __name__ == "__main__":
    parser = argparse.ArgumentParser("batchgltf2obj")
    parser.add_argument("-i", "--inputDir", help="the input directory contains the gltf models", type=str)
    parser.add_argument("-o", "--outputDir", help="the output directory for the batched output", type=str)
    parser.add_argument("-d", "--dracoCompression", nargs='?', const = True, default=False, help="enable draco compression default level 7 if provided")
    args = parser.parse_args()
    batchConvert(args.inputDir, args.outputDir, args.dracoCompression)