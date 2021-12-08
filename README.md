# gltf2obj
This is a cpp tool that convert certain gltf model to obj. This is ideal for model that is converted from obj to gltf. And for certain reason want to convert it back.

This package depends on tiny-gltf to load the draco compressed gltf file.
gltf2obj does not deal with image since the input is treated as that the textures are separated.

remember to get draco in this directory by **git clone https://github.com/google/draco.git**

cd to draco folder.

```console
$ mkdir build_dir && cd build_dir
$ cmake ../
```

Then go to gltf2obj folder


```console
$ mkdir build_dir && cd build_dir
$ cmake ../
$ cmake --build .
```

Then you should get the executable gltf2obj. Pass the gltf file Path and output dir to it, it will generate the corresponding obj model.
