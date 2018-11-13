#ifndef CG_LAB4_NODE_H
#define CG_LAB4_NODE_H
#include <vector>
#include "Mesh.h"

using namespace std;
using namespace glm;

class Node
{
public:
    string name;
    vector<Mesh> meshes;
    vector<Node> children;
    mat4 model;
    Node (mat4 model);

    void Draw (mat4 globalModel, Shader shader);
};

#endif //CG_LAB4_NODE_H
