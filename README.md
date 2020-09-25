# Shape Segmentation

Geometry processing mesh segmentation code using spectral clustering 

## Assignment Objective

- Practice using libigl to create shape decompositions using spectral clustering of various shape features
- Understand the limitations in traditional shape segmentation methods

<img src="imgs/seg-10-front.png" width=450/><img src="imgs/seg-10-side.png" width=450/>

## Compile the code 

- use cmake on Linux/OSX systems 

  `mkdir build; cd build; cmake ..; make`

- use cmake on Windows Visual Studio
  - the easies way to compile the code on windows would be **cmake_gui** which creates solution files
  - Once the solution file and project files are created without errors, you should be ready to go
  
#  Run the code

Running the code is simple but get yourself familiar with the command line options first. 
The main parameter is a shape file. You can find a few in ./mesh folder. 
The number of segments is specified by **-k** flag followed by an integer. 
You can also turn on/off visualization. Shape features can be either Shape Diameter function (SDF) or Average Geodesic Distance. 
See the usage below for more details. 

```
usage: ./build/segment [-g] [-k #] [-sdf|-geo] mesh_file (*obj or *off)
    -k #: number of segments (default is 5)
    -g: turn on visualization
    -sdf: segment using shape diameter function (default)
    -geo: segment using average geodesic distance
```

For example, try:

  `./build/segment -g -k 8 ./mesh/Centaur-1000.obj`

You will see the following images with faces randomly clustered (note: faces with the same color are in the same cluster): 

<img src="imgs/seg-random-front.png" width=450/><img src="imgs/seg-random-side.png" width=450/>


## Tasks

### 1. Compute shape features
  1. Compute shape feature for each face. The feature is a number between 0 and 1
  2. The first function is _per_face_avg_geodesic_ in per_face_feature.h/cpp. You need to compute face-to-face geodesic distances between all faces and then determine the average geodesic distance for each face as the feature for that face.
  3. The first function is _per_face_SDF_ also in per_face_feature.h/cpp. Your objective is to determine the shape diameter function of the center of every face in the mesh. Note that this is the simplified version of the original shape diamter function which represents the features using a Gaussian Mixture Model. In our case, we simplified it to a single number, namely the average length of the shape diameter. 

### 2. Compute similarity matrix


### 3. Cluster the faces using spectral clustering

### 4. Post-processing to make sure that faces in the same cluster form a single connected component

