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
    glm::vec3 Tangent;
};

struct Texture {
    unsigned int id;
};

class Mesh {

public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Material material;
    glm::mat4 localModel;

    /*  Functions  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material, Texture text);
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material, Texture text, Texture normalMap);

    Mesh() {};
    void Draw(glm::mat4 globalModel, Shader shader);
protected:
    /*  Render data  */
    unsigned int VAO, VBO, EBO;
    /*  Functions    */
    void setupMesh();
};

#endif //CG_LAB3_MESH_H
