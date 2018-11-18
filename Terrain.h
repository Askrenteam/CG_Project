#ifndef CG_LAB4_TERRAIN_H
#define CG_LAB4_TERRAIN_H


#include "Model.h"

class Terrain {
public:
    Model terrain;
    Model grass;
    Terrain(int iterations, float alpha, double resSize, double maxHeight);
    void Draw (Shader shader);
private:
    Mesh generateTerrain(int iterations, double alpha, double resSize, double maxHeight);
    Mesh generateWater(double size, double waterLevel);
    void plantGrassAt(vec3 position);
    void createGrassMesh();

    vector<Vertex> grassVerts;
    vector<unsigned int> grassIndices;
};


#endif //CG_LAB4_TERRAIN_H
