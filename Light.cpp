//
// Created by renaud on 22/11/18.
//

#include "Light.h"

Light::Light(lightType type, vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular) {
    this->type = type;
    this->position = pos;
    this->direction = dir;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}
