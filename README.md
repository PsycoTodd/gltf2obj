# **gltf2obj**
This is a cpp tool that convert certain gltf model to obj. This is ideal for model that is converted from obj to gltf. And for certain reason want to convert it back.

This package depends on tiny-gltf to load the draco compressed gltf file.
gltf2obj does not deal with image since the input is treated as that the textures are separated.

# **Build process** #

## Get draco source ##

remember to get draco in this directory by **git clone https://github.com/google/draco.git**

## Build for Mac/Linux/Windows on Comand line ##

cd to draco folder.

```console <> for optional relase build setting
$ mkdir build_dir && cd build_dir
$ cmake .. <-DCMAKE_BUILD_TYPE=Release>
$ cmake --build . <--config Release>
```

Then go to gltf2obj folder


```console
$ mkdir build_dir && cd build_dir
$ cmake .. <-DCMAKE_BUILD_TYPE=Release>
$ cmake --build . <--config Release>
```

## Build for Windows ##

Install CMake GUI version

Using the GUI to open the draco folder.
Choose the build directory to be `build_dir`
Then click configure and generate button.
Open the project in VS2015 for example. Build for the Release or Debug version.

After build complete, you should see draco.lib in build_dir/RELEASE directory.

Do the same thing for the gltf2obj.

Then you should get the executable gltf2obj. Pass the gltf file Path and output dir to it, it will generate the corresponding obj model.

## Usage ##
After build complete, you can find the executables. Try to call with one gltf with separated texture images, for example

```shell
gltf2obj.exe <input_gltf_path> <output_obj_path> <if_need_texture_conversion then 1 or 0>
```

In the output folder you will see diffuse image and obj file with mtl if you need texture conversion also. Right now we only convert diffuse texture.


# **Batch processing Examples** #

Please go to the `script` folder for python tools to convert between obj and gltf.

### obj2gltf_batch.py ###

Use this example to convert a folder containing obj sequences to gltf sequences in destination folder.

**Requirement:** have node js tool [obj2gltf](https://github.com/CesiumGS/obj2gltf) and [gltf-pipeline](https://github.com/CesiumGS/gltf-pipeline) installed in the system

```console
obj2gltf_batch.py -i <root of obj sequence> -o <root of output> [-d]
```

Provide `-d` in usual case to enable draco compression to the gltf output. If do not provide, we get gltf model without draco compression.

### gltf2obj_batch.py ###

Use this example to convert a folder that contains gltf sequences to obj model.

**Requirement:** Need to have the c++ gltf2obj tool built already and knows the path to it.

```console
gltf2obj_batch.py -e <path of the cpp gltf2obj tool> -i <root of gltf sequence> -o <root of output>
```



