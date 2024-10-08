# 3D OpenGL Rendering Project

This project demonstrates a 3D scene rendering engine using **OpenGL**, **GLFW**, and **GLEW**, along with **GLM** for handling mathematical operations such as matrix transformations. The project supports lighting, textures, shadows, and basic model rendering, and includes a custom mesh loader for rendering objects like pyramids, floors, walls, and models.

## Table of Contents
1. [Project Features](#project-features)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Directory Structure](#directory-structure)
5. [Shaders](#shaders)
6. [License](#license)

## Project Features
- **Custom Mesh Rendering**: Load and render multiple 3D objects (pyramids, walls, etc.).
- **Lighting**: Supports directional, point, and spotlight lighting with configurable materials.
- **Texture Mapping**: Objects can be textured using `.png` or `.jpg` files, with examples like brick, dirt, etc.
- **Camera Movement**: A camera system to explore the scene with adjustable perspective.
- **Shadow Mapping**: Shadows for directional and point lights for realistic rendering.

## Installation

### Prerequisites
Make sure to have the following dependencies installed:
- OpenGL
- GLEW
- GLFW
- GLM
- C++11 or later

### Clone the repository:
```bash
git clone https://github.com/DomiTe/OpenGL---Labyrinth-Prototype.git
cd OpenGL---Labyrinth-Prototype
```

### Build Instructions
For Linux/macOS, you can use `cmake` to build the project:

```bash
mkdir build
cd build
cmake ..
make
```

For Windows, follow a similar approach using `cmake` and Visual Studio.

### Running the Project
Once built, you can run the project executable:

```bash
./Source
```

## Usage
The scene contains multiple objects including pyramids and textured walls, rendered with different lighting effects. You can move the camera with:
- **W**/**A**/**S**/**D** for movement
- **Mouse** for rotating the camera
- **Space** to move up
- **Shift** to move down

The project is designed to be modular. You can add new models, lights, or textures by extending the existing framework.

## Directory Structure
```
.
├── Shaders
│   ├── shader.vert           # Vertex shader
│   └── shader.frag           # Fragment shader
├── Textures
│   ├── brick.png             # Example texture
│   └── dirt.png              # Example texture
├── src
│   ├── main.cpp              # Main application file
│   ├── Mesh.cpp              # Mesh class implementation
│   ├── Shader.cpp            # Shader loading and compiling
│   ├── Camera.cpp            # Camera implementation
│   ├── Light.cpp             # Lighting system
│   └── ...                   # Other core classes
├── include
│   ├── Mesh.h                # Mesh header file
│   ├── Shader.h              # Shader header file
│   ├── Camera.h              # Camera header file
│   └── ...                   # Other header files
└── README.md                 # This file
```

## Shaders
The project uses GLSL shaders for rendering. The shaders are located in the `Shaders` folder:
- **Vertex Shader (`shader.vert`)**: Handles vertex transformations and passes data to the fragment shader.
- **Fragment Shader (`shader.frag`)**: Handles pixel colors, textures, and lighting.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
