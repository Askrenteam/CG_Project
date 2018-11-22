#ifndef CG_LAB4_TERRAIN_H
#define CG_LAB4_TERRAIN_H


#include "Model.h"

class Terrain {
public:
    Model terrain;
    Model water;
    Model grass;
    Model windmill;
    Terrain(int iterations, float alpha, double resSize, double maxHeight);
    void Draw (Shader shader, Shader waterShader);
private:
    Mesh generateTerrain(int iterations, double alpha, double resSize, double maxHeight);
    Mesh generateWater(double size, double waterLevel);
    Model addWindmill(double waterHeight);
    void plantGrassAt(vec3 position);
    void createGrassMesh();
    double heightMin;
    double heightMax;

    vector<Vertex> grassVerts;
    vector<unsigned int> grassIndices;
};


#endif //CG_LAB4_TERRAIN_H
