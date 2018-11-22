#include "Skymap.h"
#include "TextureStore.h"

Skymap::Skymap(const char **paths) {
    this->textureID = TextureStore::instance()->loadCubeMap(paths, "skymap");
//    vector<Vertex> verts;
//    vector<unsigned int> indices;
    this->vertices.push_back({vec3(-1.0,-1.0,-1.0),vec3(0.0,0.0,0.0),vec2(0.0,0.0)});
    this->vertices.push_back({vec3(-1.0,-1.0,1.0),vec3(0.0,0.0,0.0),vec2(0.0,0.0)});
    this->vertices.push_back({vec3(1.0,-1.0,-1.0),vec3(0.0,0.0,0.0),vec2(0.0,0.0)});
    this->vertices.push_back({vec3(1.0,-1.0,1.0),vec3(0.0,0.0,0.0),vec2(0.0,0.0)});
    this->vertices.push_back({vec3(-1.0,1.0,-1.0),vec3(0.0,0.0,0.0),vec2(0.0,0.0)});
    this->vertices.push_back({vec3(-1.0,1.0,1.0),vec3(0.0,0.0,0.0),vec2(0.0,0.0)});
    this->vertices.push_back({vec3(1.0,1.0,1.0),vec3(0.0,0.0,0.0),vec2(0.0,0.0)});
    this->vertices.push_back({vec3(1.0,1.0,-1.0),vec3(0.0,0.0,0.0),vec2(0.0,0.0)});
    this->indices.push_back(0); this->indices.push_back(1); this->indices.push_back(2);
    this->indices.push_back(2); this->indices.push_back(1); this->indices.push_back(3);
    this->indices.push_back(5); this->indices.push_back(0); this->indices.push_back(1);
    this->indices.push_back(4); this->indices.push_back(0); this->indices.push_back(5);
    this->indices.push_back(5); this->indices.push_back(1); this->indices.push_back(6);
    this->indices.push_back(6); this->indices.push_back(1); this->indices.push_back(3);
    this->indices.push_back(7); this->indices.push_back(2); this->indices.push_back(3);
    this->indices.push_back(7); this->indices.push_back(3); this->indices.push_back(6);
    this->indices.push_back(7); this->indices.push_back(4); this->indices.push_back(0);
    this->indices.push_back(7); this->indices.push_back(2); this->indices.push_back(0);
    this->indices.push_back(7); this->indices.push_back(4); this->indices.push_back(5);
    this->indices.push_back(7); this->indices.push_back(5); this->indices.push_back(6);
    this->textures.push_back({textureID});
    this->setupMesh();
}

void Skymap::Draw(Shader shader) {
    glDepthMask(GL_FALSE);
    shader.Use();
    glBindVertexArray(VAO);
    shader.setUniformInt("skymap",9);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
}

void Skymap::setupMesh() {
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

    glBindVertexArray(0);

}
