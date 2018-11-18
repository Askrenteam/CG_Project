#ifndef CG_LAB3_MESH_H
#define CG_LAB3_MESH_H

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"

using namespace std;

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
//    string type;
};

class Mesh {

public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Material material;
//    MNode *node;
    glm::mat4 localModel;

    /*  Functions  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material, Texture text);
    void Draw(glm::mat4 globalModel, Shader shader);
private:
    /*  Render data  */
    unsigned int VAO, VBO, EBO;
    /*  Functions    */
    void setupMesh();
    void setupMaterial();
};

#endif //CG_LAB3_MESH_H
