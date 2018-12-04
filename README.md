# Computer Graphics Project
This is my final project for the course CS4053 - Computer Graphics at TCD.
It is developed in C++, with OpenGL. 

I used the following libraries :
- [stbi](https://github.com/nothings/stb), to load textures,
- [glm](https://glm.g-truc.net), for maths and matrices functions,
- [assimp](https://github.com/assimp/assimp), to load models.

To give credit where credit is due, I followed, for many parts of this project, the excellent tutorial at [learnOpenGL](http://learnopengl.com/).

# Features
- Random terrain generation using the Diamond-Square algorithm, with
    - Randomly distributed grass, 
    - Water plane,
- Object hierarchy (windmill w/ rotating blades),
- Phong shading (w/ directional light and mulitple point lights),
- Textures,
- Normal mapping,
- Cubemap and environment mapping (reflections on the water).

# Demo
A short and rough demo video is available on youtube :

[![Demo Video](https://img.youtube.com/vi/znXnkAyhtmo/0.jpg)](https://www.youtube.com/watch?v=znXnkAyhtmo)
