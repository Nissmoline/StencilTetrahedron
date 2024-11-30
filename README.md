# Stencil Buffer in OpenGL: Interactive 3D Scene

This repository contains the implementation of a 3D scene using the **Stencil Buffer** in OpenGL. The project features a **tetrahedron with holes cut into its faces**, a **semi-transparent cylinder enclosed within a torus**, and dynamic **scene rotation** controlled by a timer.

---

## Problem Statement

The objective of this project is to create a visually appealing 3D scene that demonstrates the use of stencil buffers for cutting holes in 3D objects and rendering semi-transparent objects inside them. The scene is dynamically animated with continuous rotation to showcase the features of OpenGL in real-time rendering.

---

## Features

1. **Tetrahedron with Holes**:
   - Holes are created using a stencil buffer to mask specific areas.
   - Accurate masking and rendering of complex cutouts.

2. **Semi-Transparent Cylinder**:
   - Rendered inside the tetrahedron and modeled as a torus.
   - Implements blending for semi-transparency effects.

3. **Scene Animation**:
   - Smooth rotation of the entire scene using a timer.
   - Adjustable rotation speed for real-time updates.

4. **Use of OpenGL Libraries**:
   - **GLFW**: Window and input handling.
   - **GLAD**: Loading OpenGL functions.
   - **GLM**: Matrix and vector calculations.

---

## Input and Output

### Input:
- Predefined vertices and indices for the tetrahedron and torus.
- Libraries integrated into the project environment.

### Output:
- A rendered 3D scene with a tetrahedron containing stencil-based holes.
- Semi-transparent torus inside the tetrahedron.
- Real-time rotation of the scene.

---

## Implementation Details

### Development Environment:
- **IDE**: Microsoft Visual Studio 2022.
- **Language**: C++.
- **Libraries**: OpenGL, GLFW, GLAD, GLM.

### Steps:

1. **Environment Setup**:
   - Install necessary libraries (GLFW, GLAD, GLM).
   - Configure Visual Studio to include paths and dependencies.

2. **Scene Creation**:
   - Define vertices and indices for the tetrahedron and torus.
   - Use stencil buffers to cut holes in the tetrahedron's faces.

3. **Rendering**:
   - Implement vertex and fragment shaders for rendering.
   - Use blending for semi-transparent effects on the torus.

4. **Animation**:
   - Update the scene's rotation angle in the main loop based on a timer.

5. **Resource Management**:
   - Clean up allocated resources after rendering.

---

## Example Code Snippets

### Tetrahedron Setup:
```cpp
float tetrahedronVertices[] = {
    1.0f,  1.0f,  1.0f,
   -1.0f, -1.0f,  1.0f,
   -1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f
};

unsigned int tetrahedronIndices[] = {
    0, 1, 2,
    0, 3, 1,
    0, 2, 3,
    1, 3, 2
};
```

### Stencil Buffer Configuration:
```cpp
glEnable(GL_STENCIL_TEST);
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
glStencilFunc(GL_ALWAYS, 1, 0xFF);
```

### Animation:
```cpp
while (!glfwWindowShouldClose(window)) {
    angle += deltaTime * 50.0f; // 50 degrees per second
    if (angle > 360.0f) angle -= 360.0f;

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotation));

    // Render scene...
}
```

---

## Requirements

- **Environment**: Visual Studio 2022.
- **Dependencies**: 
  - GLFW for window management.
  - GLAD for loading OpenGL functions.
  - GLM for mathematical operations.
- **OpenGL Version**: 3.3 or higher.

---

## Usage

1. Clone the repository:
   ```bash
   git clone: https://github.com/Nissmoline/StencilTetrahedron
   ```

2. Open the project in Visual Studio 2022.

3. Build the solution:
   - Select `Release` configuration.
   - Build using `Ctrl+Shift+B`.

4. Run the program:
   - Executable will be located in `x64/Release/StencilTetrahedron.exe`.

---

## Results

- **Visualization**: A tetrahedron with stencil-based holes and a semi-transparent torus rotating smoothly inside.
- **Performance**: Real-time rendering achieved with no errors or performance drops.

---

## License

This project is open-source and available under the MIT License. For more details.
