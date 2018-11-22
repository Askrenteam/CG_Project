#include "Node.h"

Node::Node (mat4 model) {
    this->model = model;
}

void Node::Draw(mat4 globalModel, Shader shader)
{
    this->model = globalModel * this->model;
    for(auto mesh : meshes) {
        mesh.Draw(this->model, shader);
    }
    for(auto child : children) {
        child.Draw(this->model, shader);
    }
}

void Node::translate(vec3 translation) {
    this->model = glm::translate(mat4(1.0f),translation) * this->model;
}

void Node::rotate(float degrees, vec3 axis, vec3 rotationPoint) {
    this->model = glm::translate(mat4(1.0f),-rotationPoint) * this->model;
    this->model = glm::rotate(mat4(1.0f),radians(degrees),axis) * this->model;
    this->model = glm::translate(mat4(1.0f),rotationPoint) * this->model;
}
