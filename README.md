# mesh-viewer

OPenGL applications for viewing meshes

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
mesh-viewer $ mkdir build
mesh-viewer $ cd build
mesh-viewer/build $ cmake -G "Visual Studio 16 2019" ..
mesh-viewer/build $ start CS312-MeshViewer.sln
```

Your solution file should contain multiple projects, such as `mesh-viewer`.
To run from the git bash command shell, 

```
mesh-viewer/build $ ../bin/Debug/mesh-viewer
```

*macOS*

Open terminal to the directory containing this repository.

```
mesh-viewer $ mkdir build
mesh-viewer $ cd build
mesh-viewer/build $ cmake ..
mesh-viewer/build $ make
```

To run each program from build, you would type

```
mesh-viewer/build $ ../bin/mesh-viewer
```

# Vertex Phong shading:
<img width="252" alt="example1" src="https://user-images.githubusercontent.com/54864515/115323205-a9eb6300-a155-11eb-8b2a-e1d01dcd5b48.PNG">
<img width="249" alt="example2" src="https://user-images.githubusercontent.com/54864515/115323206-a9eb6300-a155-11eb-9a70-b5e0bf9b8d5f.PNG">
<img width="248" alt="example3" src="https://user-images.githubusercontent.com/54864515/115323207-a9eb6300-a155-11eb-813c-ddaac5ef1d22.PNG">

# Fragment Phong Shading
<img width="250" alt="example4" src="https://user-images.githubusercontent.com/54864515/115323272-cedfd600-a155-11eb-99ed-7c43d3394f1a.PNG">
<img width="244" alt="example5" src="https://user-images.githubusercontent.com/54864515/115323273-cedfd600-a155-11eb-855c-1094cf0e07a9.PNG">
<img width="245" alt="example6" src="https://user-images.githubusercontent.com/54864515/115323274-cedfd600-a155-11eb-9acc-67b8d9f548e4.PNG">

