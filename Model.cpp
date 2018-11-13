#include "Model.h"

Model::Model()
{
    this->root = new Node(mat4(1.0f));
}

Model::Model(const char *path) {
    this->root = new Node(mat4(1.0f));
    loadModel(path);
}

void Model::Draw(Shader shader) {
    root->Draw(mat4(1.0f), shader);
}

void Model::loadModel(const string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene, this->root);
}

void Model::processNode(aiNode *node, const aiScene *scene, Node *currNode) {
    // process all the node's meshes (if any)
//    cout<<node->mName.C_Str()<<endl;
    currNode->name = node->mName.C_Str();
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh currMesh = processMesh(mesh, scene);
        meshes.push_back(currMesh);
        currNode->meshes.push_back(currMesh);
    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        Node *newNode = new Node(convertToMat4(node->mChildren[i]->mTransformation));
        processNode(node->mChildren[i], scene, newNode);
        currNode->children.push_back(*newNode);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    Material mat;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates

        vertex.Position = glm::vec3(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
                );
        vertex.Normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
                );
//        vertex.TexCoords = glm::vec2(
//                mesh->mTextureCoords[i]->x,
//                mesh->mTextureCoords[i]->y
//                );
        vertices.push_back(vertex);
    }

    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Load material data
    aiColor3D ambient;
    aiColor3D diffuse;
    aiColor3D specular;
    ai_real shininess;
    scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_SHININESS, shininess);
    mat.ambient = convertToVec3(ambient);
    mat.diffuse = convertToVec3(diffuse);
    mat.specular = convertToVec3(specular);
    mat.shininess = shininess;


    return Mesh(vertices, indices, mat);
}

mat4 Model::convertToMat4(aiMatrix4x4 matrix) {
    mat4 res = mat4();
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++){
            res[j][i] = matrix[i][j];
        }
    }
    return res;
}

vec3 Model::convertToVec3(aiColor3D color) {
    return vec3(color.r,color.g,color.b);
}

Node *Model::getNode(string name) {
    vector<Node*> nodeList;
    nodeList.push_back(root);
    int s = 1;
    for (int i=0 ; i < s; i++) {
        Node *node = nodeList.at(i);
//        cout<<node->name<<endl;
        if (node->name == name) return node;
        for (Node &child : node->children) {
            nodeList.push_back(&child);
            s++;
        }
    }
    return nullptr;
}