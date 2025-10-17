# FlowField3D
A project written in C++ and OpenGL that gives the opportunity to visualize vector fields with the help of particles. Expressions are parsed, converted into and evaluated in Reverse Polish notation. External dependencies are automatically fetched from source and built together with the project. For details, see [Acknowledgements](#acknowledgements)

**Note**: The project has been built and tested on **Windows 10** and **Ubuntu 24.04.2 LTS**. Windows 11 has not been tested but should work out of the box, and any further mention of Linux will refer specifically to Ubuntu 24.04.2 LTS. MacOS has not been verified as I don't have access to such a machine.

## Prerequisites

### Windows & Linux
- **C++ Compiler** - The project was built and tested with MinGW on both Windows and Linux, but others should work.
- **CMake** - Used to automate the generation of project files.
- **OpenGL** - OpenGL and appropriate drivers need to be installed.

### Linux Extras

In addition to the basic requirements, on Linux you might have to install additional packages in order to build the program. I used Ubuntu when testing and had to run the following commands:
```bash
sudo apt update && sudo apt upgrade

# Install the Mesa OpenGL development libraries, ensures proper OpenGL headers and libraries
sudo apt install libgl1-mesa-dev

# Install X11 development libraries for GLFW support
sudo apt install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

## Building
1. Clone the repository:
```bash
git clone https://github.com/Yeenasour/FlowField3D.git
cd FlowField3D
```

2. Generate the project files using CMake
```bash
mkdir build
cd build
cmake ..
```

3. Building & Running the Project
- Windows - Open the generated solution in Visual Studio or with VSCode and build the project. After build has finished, run directly through the IDE or execute the binary located in the build folder.
- Linux
```bash
make
./FlowField3D
```

## Usage
When running the project you have the following controls:
- **Mouse**
	- **Left Click & Drag**: Used for panning the current camera
	- **Scroll Wheel / Trackpad**: Used for zooming the orbital camera
- **Modifying the vector field**
    - Enter the equation in the field under the camera controls. The equation has to consist of 3 expressions, each separated by a `,`.
    - **Supported features**
        - `x`, `y` and `z` coordinates.
        - Any integer between 0 and 127, defaults to 0 if above.
        - The `+`, `-`, `*`, `/` and `^` operators.
        - `s` and `c` for sine and cosine respectively.
        - `r` for the distance from the origin, and `R` for `r²`.
        - `a` for an arbitrary constant, can be adjusted with a slider in the controls.
        - `(` and `)`.
    - **Examples**
        - `1,0,0` - constant flow of magnitude 1 along the positive x-axis.
        - `y,-x,0` - clockwise vortex around the z-axis.
        - `x/r,y/r,z/r` - attractor at the origin.
        - `1,s(x),0` - sine-wave in the positive direction of the x axis.
        - `x,-y,z` - 2 flows from the y-axis meeting and diverging at the y=0 plane.
- **Other settings**
    - `Particle Count` - controls the amount of particles, ranges from 1 to 1000.
    - `Particle Lifetime` - lifetime of each particle in seconds, ranges from 0.5 to 10.
    - `Particle Inertia` - controls how much the particle is affected by the field. 0 means that the particle has no inertia and follows the field entirely. 1 means that the particle cannot be affected by the field.
    - `Constant a` - sets the value which the constant `a` will be substituted for in an expression.
    - `Coordinate Scale` - scales the coordinate system with a constant from 1 to 10. The default axes ranges between -5 and 5, which means that it can be scaled to between -50 and 50 instead.
- **Notes**
    - To ensure that an expression evaluates correctly, make sure to be as explicit as possible. For example, write `2*(x+y)` instead of `2(x+y)`. Other cases such as `2x` or `sx` works, but to ensure consistent results choose the former whenever parenthesis are involved.

## Acknowledgements

This Project depends on the following libraries and frameworks to run:
- [**GLEW-cmake**](https://github.com/Perlmint/glew-cmake): A CMake-compatible fork of GLEW
- [**GLFW**](https://github.com/glfw/glfw): C++ library for handling window creation and managing inputs
- [**GLM**](https://github.com/g-truc/glm): OpenGL Mathematics, in this project is used minimally
- [**Dear ImGui**](https://github.com/ocornut/imgui): Library necessary for creation of GUI components, included with the project.

## License
This project is licensed under the [MIT License](LICENSE).