cmake_minimum_required(VERSION 3.12)
project(CG_Project)

set(EXTRA_LIBS
        GLEW
        GL
        glut
        assimp
        )
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLEW_INCLUDE_DIRS} )

link_directories(/usr/local/lib/)


set(CMAKE_CXX_STANDARD 14)

add_executable(CG_Project
                main.cpp
                Shader.h
                Mesh.h      Mesh.cpp
                Model.h     Model.cpp
                Node.h      Node.cpp
                Terrain.h   Terrain.cpp
                Camera.h    Camera.cpp)

target_link_libraries(CG_Project ${EXTRA_LIBS})
target_link_libraries(CG_Project ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
