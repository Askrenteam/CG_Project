#ifndef CG_PROJECT_LIGHT_H
#define CG_PROJECT_LIGHT_H

#include "glm/glm.hpp"
#include "Shader.h"
#include "Model.h"

using namespace glm;

/*
 * DIRECTIONAL : direction, no position
 * POINT : position, no direction
 * SPOT : position and direction (TODO)
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

    // Attenuation parameters for point lights
    float constant;
    float linear;
    float quadratic;

    // Luminous sphere for point lights
    Model *sphere;

    Light(lightType type, vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular);
    Light(lightType type, vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular,
            float constant, float linear, float quadratic);

    void setDirLight (Shader *shader);
    void setPointLight (Shader *shader, int index);
    void Draw (Shader shader);
};


#endif //CG_PROJECT_LIGHT_H
