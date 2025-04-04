# OpenGL Fluid Simulation

This project implements real-time fluid simulation using SPH(Smoothed-Particle Hydrodynamics) and Screen Space Rendering.
The base code and library usage is referenced in [LearnOpenGL](https://github.com/JoeyDeVries/LearnOpenGL). Check that documentation for requirements and dependency resolution.

## Demo

[![Video Label](http://img.youtube.com/vi/GX5FrHa5DLI/0.jpg)](https://youtu.be/GX5FrHa5DLI)

## Linux building
The source directory is `project` and specify the build directory as `project/build`. You can build and run it with the commands below.
```
cd project
mkdir build && cd build
cmake .. && make
./main
```

## Instructions
- Enter : Pause/Unpause simulation
- Space : Enable/Disable camera controller
- WASD : Move camera position
- Mouse : Move camera direction
- Arrow key : Increse/Decrease sun's azimuth, elevation
- Scroll : Zoom In/Out
