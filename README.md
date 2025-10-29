# VREN - Vanea's Renderer 

VREN is a simple (yet meant to be powerful) library / framework that renders stuff to the screen using OpenGL

The goal for this project is to be used in other projects as I would really love to (in the future) build any kind of physics simulation and I don't want the rendering side of things to be a limitation

# VREN SPECIFICATIONS

    1. OpenGL - used by the renderer
    2. GLAD - used to load the OpenGL functions
    3. GLFW - used for the testing side of things

# BUILD
Downloading the project and running 
```
make
```
in the terminal should build the project with no issues. If there are any issues make sure that:
    
    1. You can build ANY C++ file
    2. You have a compiler that supports '-std=c++2a' (GCC (versions 8 and later) and Clang (versions 9 and later))
    3. If none of the above work please write me an issue about it

# RUN
If the build was successful running "Launch.bat" or running the executable from the Bin folder should work