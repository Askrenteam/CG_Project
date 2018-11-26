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
vec3 lightPos;

Camera *cam;

Terrain *terr;
Skymap *sky;
Light *sun;
Light *pointLight;
Light *pointLight2;

Model *windmill;
Node *blades;

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


    mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
//    mat4 persp_proj = ortho((float)-width/40,(float)width/40,(float)-height/40,(float)height/40,0.1f,100.0f);

    skymapShader->setUniformMat4("proj", persp_proj);
    skymapShader->setUniformMat4("view", cam->view);
    sky->Draw(*skymapShader);
	// update uniforms & draw
    shader->setUniformMat4("proj", persp_proj);
    shader->setUniformMat4("view", cam->view);
//    shader->setUniformVec3("light.direction", sun->direction);
//    shader->setUniformVec3("pLight.position", pointLight->position);
    shader->setUniformVec3("eye_pos", cam->cameraPos);
    waterShader->setUniformMat4("proj", persp_proj);
    waterShader->setUniformMat4("view", cam->view);
//    waterShader->setUniformVec3("light.direction", sun->direction);
//    waterShader->setUniformVec3("pLight.position", pointLight->position);
    waterShader->setUniformVec3("eye_pos", cam->cameraPos);
//    model_data->Draw(*shader);
    terr->Draw(*shader, *waterShader);
    pointLight->Draw(*lightShader);
    pointLight2->Draw(*lightShader);
//    windmill->Draw(*shader);

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
//	lightPos = vec3 (100+200*sin(curr_time/5000), 100+200*cos(curr_time/5000), 0.0);

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

    sun = new Light(DIRECTIONAL, vec3(0,0,0), vec3(1,-0.5,0), vec3(0.3), vec3(0.8), vec3(0.6));
//    pointLight = new Light(POINT, vec3(100,40,100),vec3(0,0,0),vec3(0.3,0.0,0.0),vec3(0.8,0.5,0.5),vec3(0.5,0.1,0.1),
//                        1.0,0.022,0.0020);
//    pointLight2 = new Light(POINT, vec3(100,40,50),vec3(0,0,0),vec3(0.0,0.0,1.0),vec3(0.8,0.5,0.8),vec3(0.1,0.1,0.6),
//                        1.0,0.022,0.0020);
    pointLight = new Light(POINT, vec3(100,40000,100),vec3(0,0,0),vec3(0.3,0.0,0.0),vec3(0.8,0.5,0.5),vec3(0.5,0.1,0.1),
                        1.0,0.022,0.0020);
    pointLight2 = new Light(POINT, vec3(100,40000,50),vec3(0,0,0),vec3(0.0,0.0,1.0),vec3(0.8,0.5,0.8),vec3(0.1,0.1,0.6),
                        1.0,0.022,0.0020);

	waterShader = new Shader("../shaders/waterVertexShader.vs","../shaders/waterFragmentShader.frag");
    lightShader = new Shader("../shaders/lightVertexShader.vs","../shaders/lightFragmentShader.frag");


    terr = new Terrain(8,30.0,200,100.0);
//    terr = new Terrain(8,50.0,200,200.0);
//	terr = new Terrain(1,2,20.0,5.0);
    blades = terr->windmill.getNode("Pales");
    cam = new Camera(vec3(1,0,0),vec3(-1.0,0.0,0.0),vec3(0.0,0.0,0.0),0.5f,0.25f);

    sun->setDirLight(shader);
    sun->setDirLight(waterShader);
    pointLight->setPointLight(shader,0);
    pointLight->setPointLight(waterShader,0);
    pointLight2->setPointLight(shader,1);
    pointLight2->setPointLight(waterShader,1);

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
