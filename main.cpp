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
Shader *glassShader;
vec4 eyePos;
vec3 lightPos;

Camera *cam;

Model *monkey;
Terrain *terr;

//ModelData mesh_data;
Model *model_data;
Node *right_upper_arm;
Node *right_arm;
Node *left_upper_arm;
Node *left_arm;
Node *left_hand;
Node *left_leg;
Node *right_leg;
Node *head;
unsigned int mesh_vao = 0;
int width = 800;
int height = 600;
int xC = 0;
int yC = 0;


GLuint loc1, loc2, loc3;
GLfloat rotate_y = 0.0f;



void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_BLEND); // enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE); // disable suppression of backfaces;
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.207f, 0.588f, 0.878f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader->id);


	// Root of the Hierarchy
    mat4 view = lookAt(vec3(eyePos.x,eyePos.y,eyePos.z),vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
//    mat4 persp_proj = ortho((float)-width/40,(float)width/40,(float)-height/40,(float)height/40,0.1f,100.0f);

	// update uniforms & draw
    shader->setUniformMat4("proj", persp_proj);
    shader->setUniformMat4("view", cam->view);
    shader->setUniformVec3("LightPosition", lightPos);
    shader->setUniformVec3("eye_pos", eyePos);
//    model_data->Draw(*shader);
    terr->model.Draw(*shader);
//    glassShader->setUniformMat4("proj", persp_proj);
//    glassShader->setUniformMat4("view", view);
//    glassShader->setUniformVec3("LightPosition", lightPos);
//    glassShader->setUniformVec3("eye_pos", eyePos);
//
//    monkey->Draw(*glassShader);

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
	lightPos = vec3 (40*sin(curr_time/1000), 40*cos(curr_time/1000), 10.0);
	// Draw the next frame
	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	shader = new Shader("../simpleVertexShader.vs","../simpleFragmentShader.frag");
	glassShader = new Shader("../glassVertexShader.vs","../glassFragmentShader.frag");

	terr = new Terrain(8,30.0,300.0);
	cam = new Camera(vec3(50,50,300),vec3(0.0,0.0,1.0),vec3(0.0,150.0,0.0),0.5f,0.15f);
	monkey = new Model("../sphere.dae");
	monkey->root->model = translate(monkey->root->model,vec3(-8.0,0.0,0.0));
	// load model
    model_data = new Model("../coca/bottle.3ds");
//    model_data = new Model("../robot.dae");
    right_upper_arm =  model_data->getNode("right_upper_arm");
    right_arm =  model_data->getNode("right_arm");
    right_leg =  model_data->getNode("right_leg");
    left_upper_arm =  model_data->getNode("left_upper_arm");
    left_arm =  model_data->getNode("left_arm");
    left_hand =  model_data->getNode("left_hand");
    left_leg =  model_data->getNode("left_leg");
    head =  model_data->getNode("head");

    eyePos = vec4(-4.0f, -4.0f, 4.0f, 1.0f);
    lightPos = vec3 (50.0, 300.0, 100.0);
    shader->setUniformVec3("light.ambient", vec3(0.2));
    shader->setUniformVec3("light.diffuse", vec3(0.8));
    shader->setUniformVec3("light.specular", vec3(1.0));
    glassShader->setUniformVec3("light.ambient", vec3(0.2));
    glassShader->setUniformVec3("light.diffuse", vec3(0.8));
    glassShader->setUniformVec3("light.specular", vec3(1.0));
}

void mousemove(int x, int y) {
    cam->rotateCamera(x, y);
}

void keypress(unsigned char key, int x, int y) {
	vec3 rot;
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
		case '+':
			eyePos = scale(mat4(1.0f),vec3(0.75f)) * eyePos;
			break;
		case '-':
			eyePos = scale(mat4(1.0f),vec3(1.5f)) * eyePos;
			break;
        case 'o':
            model_data->root->model = rotate(model_data->root->model,radians(-10.0f),vec3(0.0f,0.0f,1.0f));
            break;
        case 'p':
            model_data->root->model = rotate(model_data->root->model,radians(10.0f),vec3(0.0f,0.0f,1.0f));
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
        case '0':
            model_data->root->model = translate(model_data->root->model,vec3(0.0f,0.0f,-1.0f));
            break;
        case 'l':
            right_upper_arm->model = rotate(mat4(1.0f),radians(-10.0f),vec3(0.0f,0.0f,1.0f))
                                     * right_upper_arm->model;
            break;
        case 'm':
            right_upper_arm->model = rotate(mat4(1.0f),radians(10.0f),vec3(0.0f,0.0f,1.0f))
                                     * right_upper_arm->model;
            break;
        case 'j':
            right_arm->model = rotate(mat4(1.0f),radians(-10.0f),vec3(0.0f,0.0f,1.0f))
                                     * right_arm->model;
            break;
        case 'k':
            right_arm->model = rotate(mat4(1.0f),radians(10.0f),vec3(0.0f,0.0f,1.0f))
                                     * right_arm->model;
            break;
        case 'b':
            left_upper_arm->model = rotate(mat4(1.0f),radians(-10.0f),vec3(0.0f,0.0f,1.0f))
                                    * left_upper_arm->model;
            break;
        case 'n':
            left_upper_arm->model = rotate(mat4(1.0f),radians(10.0f),vec3(0.0f,0.0f,1.0f))
                                    * left_upper_arm->model;
            break;
        case 'c':
            left_arm->model = rotate(mat4(1.0f),radians(-10.0f),vec3(0.0f,0.0f,1.0f))
                              * left_arm->model;
            break;
        case 'v':
            left_arm->model = rotate(mat4(1.0f),radians(10.0f),vec3(0.0f,0.0f,1.0f))
                              * left_arm->model;
            break;
        case 'w':
            left_hand->model = rotate(mat4(1.0f),radians(-10.0f),vec3(0.0f,0.0f,1.0f))
                              * left_hand->model;
            break;
        case 'x':
            left_hand->model = rotate(mat4(1.0f),radians(10.0f),vec3(0.0f,0.0f,1.0f))
                              * left_hand->model;
            break;
        case 'u':
            head->model = rotate(mat4(1.0f),radians(-10.0f),vec3(0.0f,0.0f,1.0f))
                              * head->model;
            break;
        case 'i':
            head->model = rotate(mat4(1.0f),radians(10.0f),vec3(0.0f,0.0f,1.0f))
                              * head->model;
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
