MyVoronoi is used to compute Voronoi Diagram for point cloud.&#x20;

## Installing and Compiling

All the dependencies of the project are CGAL, Libigl, Eigen, GMP. In Windows, the project requires Visual Studio 2022.

The project is managed with CMake. First, clone the repository in your libraries folder to add the library. Then add the following variables to your system variables:\

```markup
CGAL_DIR          D:\dev\CGAL-5.6\include
GMP_INCLUDE_DIR   D:\dev\CGAL-5.6\auxiliary\gmp\include
GMP_LIBRARIES     D:\dev\CGAL-5.6\auxiliary\gmp\lib
```

and add D:\dev\CGAL-5.6, D:\dev\CGAL-5.6\auxiliary\gmp\lib to Path. Notice: directories are need to be your own path. So do for the directories in CMakeLists.txt Line 25 and 28.

### As an executable

Open a console inside the project folder and execute the following commands:

```markup
mkdir build
cd build
cmake ..
```

Inside Visual Studio, open the project by searching for the `MyVoronoi.sln` file.&#x20;
