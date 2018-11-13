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
