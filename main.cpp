#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include "Model.h"
#include "Terrain.h"
#include "Camera.h"
#include "TextureStore.h"



/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_NAME "monkeyhead_smooth.dae"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/
using namespace std;
using namespace glm;


#pragma region SimpleTypes
typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;
#pragma endregion SimpleTypes

Shader *shader;
vec3 lightPos;

Camera *cam;

Terrain *terr;

//ModelData mesh_data;
int width = 800;
int height = 600;
int xC = 0;
int yC = 0;


void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
//	glEnable(GL_BLEND); // enable alpha blending
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDisable(GL_CULL_FACE); // disable suppression of backfaces;
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.207f, 0.588f, 0.878f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader->id);


	// Root of the Hierarchy
//    mat4 view = lookAt(vec3(eyePos.x,eyePos.y,eyePos.z),vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
//    mat4 persp_proj = ortho((float)-width/40,(float)width/40,(float)-height/40,(float)height/40,0.1f,100.0f);

	// update uniforms & draw
    shader->setUniformMat4("proj", persp_proj);
    shader->setUniformMat4("view", cam->view);
    shader->setUniformVec3("LightPosition", lightPos);
    shader->setUniformVec3("eye_pos", cam->cameraPos);
//    model_data->Draw(*shader);
    terr->Draw(*shader);

	glutSwapBuffers();
}


void updateScene() {

	static double last_time = 0;
	double curr_time = glutGet(GLUT_ELAPSED_TIME);
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

//	// Rotate the light around the model
//	lightPos = vec3 (10+100*sin(curr_time/5000), 10+100*cos(curr_time/5000), 0.0);
	// Draw the next frame
	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	shader = new Shader("../simpleVertexShader.vs","../simpleFragmentShader.frag");

    TextureStore::instance()->loadTexture("../grass.png", "grass");
    TextureStore::instance()->loadTexture("../grass_blade.png", "blade");
    TextureStore::instance()->loadTexture("../water.png", "water");

	terr = new Terrain(8,30.0,200,100.0);
//	terr = new Terrain(1,2,20.0,5.0);


	cam = new Camera(vec3(1,0,0),vec3(-1.0,0.0,0.0),vec3(0.0,0.0,0.0),0.5f,0.25f);

    lightPos = vec3 (100.0, 100.0, 100.0);
    shader->setUniformVec3("light.ambient", vec3(0.3));
    shader->setUniformVec3("light.diffuse", vec3(1.0));
    shader->setUniformVec3("light.specular", vec3(0.6));
}

void mousemove(int x, int y) {
    cam->rotateCamera(x, y);
}

void keypress(unsigned char key, int x, int y) {
	switch(key)
    {
        case 27: //Escape key
            exit(0);
		case 'q':
            cam->moveCamera(0.0,3.0,0.0);
            break;
        case 'd':
            cam->moveCamera(0.0,-3.0,0.0);
            break;
        case 'z':
            cam->moveCamera(3.0,0.0,0.0);
            break;
        case 's':
            cam->moveCamera(-3.0,0.0,0.0);
            break;
        case 'x':
            cam->moveCamera(0.0,0.0,3.0);
            break;
        case 'w':
            cam->moveCamera(0.0,0.0,-3.0);
            break;
        case '8':
            yC-=5;
            cam->rotateCamera(xC, yC);
            break;
        case '4':
            xC-=5;
            cam->rotateCamera(xC, yC);
            break;
        case '6':
            xC+=5;
            cam->rotateCamera(xC, yC);
            break;
        case '2':
            yC+=5;
            cam->rotateCamera(xC, yC);
            break;
		default:
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glEnable(GL_BLEND);
	glutInitWindowSize(width, height);
    glutInitContextVersion(4, 5);
    glutCreateWindow("Hello Triangle");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);

//    glutPassiveMotionFunc( mousemove );
//    glutMotionFunc( mousemove );
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
