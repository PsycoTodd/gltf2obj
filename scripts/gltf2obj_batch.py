import argparse
import os
import subprocess
import shutil

#notice that this mtl conversion is specific for the target data set form.
#it may not be general for all mtl cases.
def convertMtl(mtlString, keyword):
    output = ''
    containImg = False
    for line in mtlString.split('\n'):
        if line.__contains__('newmtl'):
            output += 'newmtl ' + keyword
        elif line.__contains__('.jpg'):
            preFix = line.rsplit(' ', 1)[0]
            output += preFix + ' ' + keyword + '.jpg'
            containImg = True
        else:
            output += line
        output += '\n'
    return output, containImg


def batchConvert(exePath, inputGltfDir, outputObjDir, mtlTemplate):
    mtlData = ''
    if os.path.exists(mtlTemplate):
        mtlFile = open(mtlTemplate, 'r')
        mtlData = mtlFile.read()
        mtlFile.close()
    for file in os.listdir(inputGltfDir):
        objConverted = False
        mtlConverted = False
        if file.endswith(".gltf"):
            outputObjFilePath = os.path.join(outputObjDir, file.replace("gltf", "obj"))
            if not os.path.exists(outputObjFilePath):
                args = ( exePath, os.path.join(inputGltfDir, file),  outputObjFilePath)
                popen = subprocess.Popen(args, stdout=subprocess.PIPE)
                popen.wait()
                objConverted = True
            # now process to covert mtl files
            if mtlData:
                outputMtlFilePath = os.path.join(outputObjDir, file.replace('gltf', 'mtl'))
                if not os.path.exists(outputMtlFilePath):
                    curMtlData, containImg = convertMtl(mtlData, file.rsplit('.', 1)[0])
                    with open(outputMtlFilePath, 'w') as curMtlFile:
                        curMtlFile.write(curMtlData)
                    if containImg:
                        outputImgFileName = file.replace('.gltf', '.jpg')
                        inputImgPath = os.path.join(inputGltfDir, outputImgFileName)
                        outputImgPath = os.path.join(outputObjDir, outputImgFileName)
                        if os.path.exists(inputImgPath) and not os.path.exists(outputImgPath):
                            shutil.copy(inputImgPath, outputImgPath)
                    mtlConverted = True
            if objConverted or mtlConverted:
                print("converted " + file)

if __name__ == "__main__":
    parser = argparse.ArgumentParser("batchgltf2obj")
    parser.add_argument("-e", "--gltf2obj", help="the path to the built gltf2obj executable", type=str)
    parser.add_argument("-i", "--inputDir", help="the input directory contains the gltf models", type=str)
    parser.add_argument("-o", "--outputDir", help="the output directory for the batched output", type=str)
    parser.add_argument("-m", "--mtlTemplate", help="if we need to also convert to get the obj model's mtl file", type=str, required=False, default='')
    args = parser.parse_args()
    batchConvert(args.gltf2obj, args.inputDir, args.outputDir, args.mtlTemplate)
