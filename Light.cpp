#include "Light.h"

Light::Light(lightType type, vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular) {
    this->type = type;
    this->position = pos;
    this->direction = dir;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

Light::Light(lightType type, vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular, float constant,
             float linear, float quadratic) {
    this->type = type;
    this->position = pos;
    this->direction = dir;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;

    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

    this->sphere = new Model("../res/sphere.dae");
}

void Light::setDirLight(Shader *shader) {
    if (type != DIRECTIONAL) return;
    shader->setUniformVec3("light.direction", this->direction);
    shader->setUniformVec3("light.ambient", this->ambient);
    shader->setUniformVec3("light.diffuse", this->diffuse);
    shader->setUniformVec3("light.specular", this->specular);
}

void Light::setPointLight(Shader *shader, int index) {
    if (type != POINT) return;
    string pLight = "pLights["+to_string(index)+"].";
    shader->setUniformVec3((pLight+"position").c_str(), this->position);
    shader->setUniformVec3((pLight+"ambient").c_str(), this->ambient);
    shader->setUniformVec3((pLight+"diffuse").c_str(), this->diffuse);
    shader->setUniformVec3((pLight+"specular").c_str(), this->specular);
    shader->setUniformFloat((pLight+"constant").c_str(), this->constant);
    shader->setUniformFloat((pLight+"linear").c_str(), this->linear);
    shader->setUniformFloat((pLight+"quadratic").c_str(), this->quadratic);
}

void Light::Draw(Shader shader) {
    shader.setUniformVec3("lightColor", this->diffuse);
    this->sphere->Draw(shader);
}
