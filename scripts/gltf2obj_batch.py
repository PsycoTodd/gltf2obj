import argparse
import os
import subprocess

def batchConvert(exePath, inputGltfDir, outputObjDir):
    for file in os.listdir(inputGltfDir):
        if file.endswith(".gltf"):
            args = ( exePath, os.path.join(inputGltfDir, file), os.path.join(outputObjDir, file.replace("gltf", "obj")) )
            popen = subprocess.Popen(args, stdout=subprocess.PIPE)
            popen.wait()
            print("converted " + file)

if __name__ == "__main__":
    parser = argparse.ArgumentParser("batchgltf2obj")
    parser.add_argument("-e", "--gltf2obj", help="the path to the built gltf2obj executable", type=str)
    parser.add_argument("-i", "--inputDir", help="the input directory contains the gltf models", type=str)
    parser.add_argument("-o", "--outputDir", help="the output directory for the batched output", type=str)
    parser.add_argument("-m", "--mtlTemplate", help="if we need to also convert to get the obj model's mtl file", type=str, nargs='?', default='')
    args = parser.parse_args()
    batchConvert(args.gltf2obj, args.inputDir, args.outputDir)
