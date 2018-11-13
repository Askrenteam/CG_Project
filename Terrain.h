#ifndef CG_LAB4_TERRAIN_H
#define CG_LAB4_TERRAIN_H


#include "Model.h"

class Terrain {
public:
    Model model;
    Terrain(int iterations, float alpha, double maxHeight);

private:
    Mesh generateTerrain(int iterations, double alpha, double maxHeight);
};


#endif //CG_LAB4_TERRAIN_H
