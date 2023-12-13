# FOFEM C++ code
First Order Fire Effects Model (FOFEM) source code, DLL, and UNIX installations.

### How to install using CMake
These instructions compile the code in folder FOF_UNIX.

1. Create a directory where to build the code
```
mkdir build
cd build
```
2. Create Makefile using CMake by doing:
```
cmake ..
```
3. Install FOFEM code by doing:
```
make
```

This creates an executable called fofem in the same build folder. You can run the code by doing:
```
./fofem
```
After modifying the code in the FOF_UNIX folder, you can just go to the build folder and type make again to update the installation.
