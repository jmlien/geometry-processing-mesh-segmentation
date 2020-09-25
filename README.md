# Shape Segmentation

Geometry processing mesh segmentation code using spectral clustering 

Objective: Practice using libigl to create shape decompositions using spectral clustering of various shape features

<img src="imgs/seg-random-front.png" width=450/> <img src="imgs/seg-random-side.png" width=450/>

# Compile the code 

- use cmake on Linux/OSX systems 

  `mkdir build; cd build; cmake ..; make`

- use cmake on Windows Visual Studio
  - the easies way will be through cmake_gui and create solution files through cmake
  
#  Run the code

Running the code is simple but get yourself familiar with the command line option. 
The main parameter is the shape file. You can find a few in ./mesh folder. 
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
