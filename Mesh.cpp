#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material, Texture text) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures.push_back(text);
    this->localModel = glm::mat4(1.0f);
    this->material = material;

    setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material, Texture text, Texture normalMap) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures.push_back(text);
    this->textures.push_back(normalMap);
    this->localModel = glm::mat4(1.0f);
    this->material = material;

    setupMesh();

}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex textures
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glBindVertexArray(0);
}


void Mesh::Draw(glm::mat4 globalModel, Shader shader) {
    globalModel = globalModel * this->localModel;

    glBindVertexArray(VAO);
    shader.setUniformInt("tex",textures.at(0).id);
    glBindTexture(GL_TEXTURE_2D, textures.at(0).id);
    if (textures.size() > 1) {
        shader.setUniformInt("normalMap",textures.at(1).id);
        glBindTexture(GL_TEXTURE_2D, textures.at(1).id);
    }
    if (textures.size() > 2) {
        shader.setUniformInt("skymap",9);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textures.at(2).id);
    }
    shader.setUniformMat4("model", globalModel);
    shader.setUniformMat3("t_i_model", glm::mat3(glm::transpose(glm::inverse(globalModel))));
    shader.setUniformVec3("matrl.ambient", this->material.ambient);
    shader.setUniformVec3("matrl.diffuse", this->material.diffuse);
    shader.setUniformVec3("matrl.specular", this->material.specular);
    shader.setUniformFloat("matrl.shininess", this->material.shininess);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
