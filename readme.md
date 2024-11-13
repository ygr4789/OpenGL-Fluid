# OpenGL Fluid Simulation
This project implements real-time fluid simulation using Smoothed-Particle Hydrodynamics (SPH) and Screen Space Rendering.
The base code and library usage is referenced in [LearnOpenGL](https://github.com/JoeyDeVries/LearnOpenGL/tree/master). Check that documentation for requirements and dependency resolution.

## Linux building
First make sure you have CMake, Git, and GCC by typing as root (sudo) `apt-get install g++ cmake git` and then get the required packages:
Using root (sudo) and type `apt-get install libsoil-dev libglm-dev libassimp-dev libglew-dev libglfw3-dev libxinerama-dev libxcursor-dev  libxi-dev libfreetype-dev libgl1-mesa-dev xorg-dev` .

**Build through CMake-gui:** The source directory is `project` and specify the build directory as `LearnOpenGL/build`. You can build and run it with the commands below.

**Build through Cmake command line:**
```
cd /path/to/LearnOpenGL
mkdir build && cd build
cmake .. && make
./main
```