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
#include "Skymap.h"
#include "Light.h"



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


Shader *shader;
Shader *skymapShader;
Shader *waterShader;
Shader *lightShader;

Camera *cam;

Terrain *terr;
Skymap *sky;
Light *sun;
#define nrLights 2
Light *pointLights [nrLights];

Node *blades;

bool paused = false; // To pause time

//int width = 800;
//int height = 600;
int width = 1366;
int height = 768;
int xC = 0;
int yC = 0;


void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.207f, 0.588f, 0.878f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sun->setDirLight(shader);
    sun->setDirLight(waterShader);
    for (int i=0; i<nrLights; i++) {
        pointLights[i]->setPointLight(shader,i);
        pointLights[i]->setPointLight(waterShader,i);
    }


    mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

    skymapShader->setUniformMat4("proj", persp_proj);
    skymapShader->setUniformMat4("view", cam->view);
    sky->Draw(*skymapShader);
	// update uniforms & draw
    shader->setUniformMat4("proj", persp_proj);
    shader->setUniformMat4("view", cam->view);
    shader->setUniformVec3("eye_pos", cam->cameraPos);
    waterShader->setUniformMat4("proj", persp_proj);
    waterShader->setUniformMat4("view", cam->view);
    waterShader->setUniformVec3("eye_pos", cam->cameraPos);
    lightShader->setUniformMat4("proj", persp_proj);
    lightShader->setUniformMat4("view", cam->view);
    lightShader->setUniformVec3("eye_pos", cam->cameraPos);

    terr->Draw(*shader, *waterShader);
    pointLights[0]->Draw(*lightShader);
    pointLights[1]->Draw(*lightShader);

	glutSwapBuffers();
}


void updateScene() {

	static double last_time = 0;
	double curr_time = glutGet(GLUT_ELAPSED_TIME);
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

    // Rotate the light around the model
	if (!paused) sun->direction = vec3(sin(curr_time/4000),-cos(curr_time/4000),0);

    // Rotate the windmill's blades
    blades->rotate(1.0f, vec3(0.0,0.0,1.0), vec3(0.205,875,0.0));
    // Draw the next frame
	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	shader = new Shader("../shaders/simpleVertexShader.vs","../shaders/simpleFragmentShader.frag");
	skymapShader = new Shader("../shaders/skymapVertexShader.vs","../shaders/skymapFragmentShader.frag");
    waterShader = new Shader("../shaders/waterVertexShader.vs","../shaders/waterFragmentShader.frag");
    lightShader = new Shader("../shaders/lightVertexShader.vs","../shaders/lightFragmentShader.frag");


    // Load textures
    TextureStore::instance()->loadTexture("../res/grass.png", "grass",0);
    TextureStore::instance()->loadTexture("../res/cross_texture.png", "cross",0);
    TextureStore::instance()->loadTexture("../res/random_normal_map.png", "normalMap",2);
    TextureStore::instance()->loadTexture("../res/grass_blade.png", "blade",0);
    TextureStore::instance()->loadTexture("../res/water.png", "water",0);
    const char* faces[] = {"../res/skybox/right.jpg",
                           "../res/skybox/left.jpg",
                           "../res/skybox/top.jpg",
                           "../res/skybox/bottom.jpg",
                           "../res/skybox/front.jpg",
                           "../res/skybox/back.jpg"};

    sky = new Skymap(faces);

    // Create lights
    sun = new Light(DIRECTIONAL, vec3(0,0,0), vec3(1,-0.5,0), vec3(0.2), vec3(0.9), vec3(0.6));
    pointLights[0] = new Light(POINT, vec3(100,40,100),vec3(0,0,0),vec3(0.3,0.0,0.0),vec3(0.8,0.5,0.5),vec3(1.0,0.7,0.7),
                        1.0,0.014,0.0007);
    pointLights[1] = new Light(POINT, vec3(100,40,50),vec3(0,0,0),vec3(0.0,0.0,1.0),vec3(0.8,0.5,0.8),vec3(0.1,0.1,0.6),
                        1.0,0.022,0.0020);
    // Link the lights to the shaders
    sun->setDirLight(shader);
    sun->setDirLight(waterShader);
    for (int i=0; i<nrLights; i++) {
        pointLights[i]->setPointLight(shader,i);
        pointLights[i]->setPointLight(waterShader,i);
    }



    terr = new Terrain(8,80.0,200,200.0);
    blades = terr->windmill.getNode("Pales");

    cam = new Camera(vec3(1,0,0),vec3(-1.0,0.0,0.0),vec3(0.0,0.0,0.0),0.5f,0.25f);
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
        case 'o':
            pointLights[0]->move(vec3(0.0,1.0,0.0));
            break;
        case 'l':
            pointLights[0]->move(vec3(0.0,-1.0,0.0));
            break;
        case 'k':
            pointLights[0]->move(vec3(1.0,0.0,0.0));
            break;
        case 'm':
            pointLights[0]->move(vec3(-1.0,0.0,0.0));
            break;
        case 'i':
            pointLights[0]->move(vec3(0.0,0.0,1.0));
            break;
        case 'p':
            pointLights[0]->move(vec3(0.0,0.0,-1.0));
            break;
        case ' ':
            paused = !paused;
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

	// Mouse camera movement
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
