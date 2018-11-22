#ifndef CG_PROJECT_LIGHT_H
#define CG_PROJECT_LIGHT_H

#include "glm/glm.hpp"

using namespace glm;

/*
 * DIRECTIONAL : direction, no position
 * POINT : position, no direction
 * SPOT : position and direction
 */
enum lightType {DIRECTIONAL, POINT, SPOT};

class Light {
    public:
    lightType type;
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    Light(lightType type, vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular);

};


#endif //CG_PROJECT_LIGHT_H
