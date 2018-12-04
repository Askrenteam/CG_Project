#include "Terrain.h"
#include "TextureStore.h"

Terrain::Terrain(int iterations, float alpha, double resSize, double maxHeight)
{
    this->terrain = Model();
    this->water = Model();
    this->grass = Model();
    terrain.root->meshes.push_back(generateTerrain(iterations, alpha, resSize,  maxHeight));
    double waterHeight = heightMin + (heightMax-heightMin)/2;
    water.root->meshes.push_back(generateWater(resSize, waterHeight));
    water.root->meshes.at(0).textures.push_back({TextureStore::instance()->getTexture("skymap")});
    this->windmill = addWindmill(waterHeight);
    cout<<"Terrain created"<<endl;
}

// returns a random float between 0 and a
float mRandom(float a) {
    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/a));
}

Mesh Terrain::generateTerrain(int n, double alpha, double resSize, double maxHeight) {
    int size = (int)pow(2,n)+1;
    double heightMap[size][size];
    for (int x=0; x<size; x++) {
        for (int y = 0; y < size; y++) {
            heightMap[x][y] = 0.0;
        }
    }

    srand(time(0));
    heightMap[0][0] = mRandom(maxHeight);
    heightMap[0][size-1] = mRandom(maxHeight);
    heightMap[size-1][0] = mRandom(maxHeight);
    heightMap[size-1][size-1] = mRandom(maxHeight);
    for (int i=0; i<n; i++) {
        // Diamond
        int step = (int)((size-1)/pow(2,i+1));

        for (int x=step; x <size-1; x+=2*step) {
            for ( int y=step; y <size-1; y+=2*step) {
                heightMap[x][y] = ((mRandom(2*alpha))-alpha+
                                  heightMap[x-step][y-step] +
                                   heightMap[x+step][y-step] +
                                   heightMap[x+step][y+step] +
                                   heightMap[x-step][y+step]) / 4;
            }
        }

        // Square
        for (int x=step; x <size; x+=2*step) {
            heightMap[x][0] = ((mRandom(2*alpha))-alpha+
                              heightMap[x-step][0] +
                               heightMap[x+step][0] +
                               heightMap[x][step] ) / 3;
            for (int y=2*step; y<size-2*step; y+= 2*step) {
                heightMap[x][y] = ((mRandom(2*alpha))-alpha+
                                  heightMap[x-step][y] +
                                   heightMap[x+step][y] +
                                   heightMap[x][y-step] +
                                   heightMap[x][y+step]) / 4;
            }
            heightMap[x][size-1] = (mRandom(2*alpha))-alpha+
                              (heightMap[x-step][size-1] +
                               heightMap[x+step][size-1] +
                               heightMap[x][size-1-step] ) / 3;
        }
        for (int y=step; y <size; y+=2*step) {
            heightMap[0][y] = ((mRandom(2*alpha))-alpha+
                              heightMap[0][y-step] +
                               heightMap[0][y+step] +
                               heightMap[step][y] ) / 3;
            for (int x=2*step; x<size-2*step; x+= 2*step) {
                heightMap[x][y] = ((mRandom(2*alpha))-alpha+
                                  heightMap[x-step][y] +
                                   heightMap[x+step][y] +
                                   heightMap[x][y-step] +
                                   heightMap[x][y+step]) / 4;
            }
            heightMap[size-1][y] = ((mRandom(2*alpha))-alpha+
                                   heightMap[size-1][y-step] +
                                    heightMap[size-1][y+step] +
                                    heightMap[size-1-step][y] ) / 3;
        }

        alpha *= 0.5;
    }

    Material mat = {vec3(0.3,0.3,0.3),
                    vec3(0.8,0.8,0.8),
                    vec3(0.1,0.1,0.1),
                    2};
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    unsigned int currVert = 0;
    double resizeFactor = resSize/(float)size;
    heightMin = DBL_MAX;
    heightMax = -DBL_MAX;
    for (int x=0; x<size; x++) {
        for (int y=0; y<size; y++) {
            if (heightMap[x][y] > heightMax) heightMax = heightMap[x][y];
            if (heightMap[x][y] < heightMin) heightMin = heightMap[x][y];

            Vertex vert;
            vert.Position = vec3(x*resizeFactor,heightMap[x][y]*resizeFactor,y*resizeFactor);
            plantGrassAt(vec3(vert.Position.x+mRandom(resizeFactor)-resizeFactor/2,
                              vert.Position.y,
                              vert.Position.z+mRandom(resizeFactor)-resizeFactor/2));
            plantGrassAt(vec3(vert.Position.x+mRandom(resizeFactor)-resizeFactor/2,
                              vert.Position.y,
                              vert.Position.z+mRandom(resizeFactor)-resizeFactor/2));

            vec3 n1 = vec3(0,0,0);
            vec3 n2 = vec3(0,0,0);
            vec3 n3 = vec3(0,0,0);
            vec3 n4 = vec3(0,0,0);
            if (x != 0) {
                float a = heightMap[x-1][y] - heightMap[x][y];
                n1 = vec3(a/sqrt(a*a+1),1/sqrt(a*a+1),0);
            }
            if (x != size-1) {
                float b = heightMap[x][y] - heightMap[x+1][y];
                n2 = vec3(b/sqrt(b*b+1),1/sqrt(b*b+1),0);
                if(y!=size-1){
                    indices.push_back(currVert);
                    indices.push_back(currVert+size);
                    indices.push_back(currVert+1);
                    indices.push_back(currVert+size);
                    indices.push_back(currVert+size+1);
                    indices.push_back(currVert+1);
                }
            }
            if (y != 0) {
                float c = heightMap[x][y-1] - heightMap[x][y];
                n3 = vec3(0,1/sqrt(c*c+1),c/sqrt(c*c+1));
            }
            if (y != size-1) {
                float d = heightMap[x][y] - heightMap[x][y+1];
                n4 = vec3(0,1/sqrt(d*d+1),d/sqrt(d*d+1));
            }
            vert.Normal = normalize(n1+n2+n3+n4);
            vert.TexCoords = vec2(x*resizeFactor,y*resizeFactor);
            vert.Tangent = normalize(cross(vec3(1,0,0),vert.Normal));
            vertices.push_back(vert);
            currVert++;
        }
    }
    heightMax *= resizeFactor;
    heightMin *= resizeFactor;
    createGrassMesh();
    Texture tex = {TextureStore::instance()->getTexture("grass")};
    Texture normalMap = {TextureStore::instance()->getTexture("normalMap")};
    return Mesh(vertices, indices, mat, tex, normalMap);

}

Mesh Terrain::generateWater(double size, double waterLevel) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    Material mat = {vec3(0.3,0.3,0.3),
                    vec3(0.3,0.3,0.3),
                    vec3(0.9,0.9,0.9),
                    32};

    vertices.push_back({vec3(0,waterLevel,0),vec3(0.0,1.0,0.0), vec2(0.0,size),vec3(1,0,0)});
    vertices.push_back({vec3(0,waterLevel,size-1),vec3(0.0,1.0,0.0), vec2(0.0,0.0),vec3(1,0,0)});
    vertices.push_back({vec3(size-1,waterLevel,0),vec3(0.0,1.0,0.0), vec2(size,size),vec3(1,0,0)});
    vertices.push_back({vec3(size-1,waterLevel,size-1),vec3(0.0,1.0,0.0), vec2(size,0.0),vec3(1,0,0)});
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(3);
    Texture tex = {TextureStore::instance()->getTexture("water")};
    Texture normalMap = {TextureStore::instance()->getTexture("normalMap")};
    return Mesh(vertices, indices, mat, tex, normalMap);
}

Model Terrain::addWindmill(double waterHeight) {
    Model windmill = Model("../res/WMobj.dae");
    Node *house = windmill.getNode("WindMill");
    house->meshes.at(0).textures.clear();
    house->meshes.at(0).textures.push_back({TextureStore::instance()->loadTexture("../res/windmill_diffuse.tga", "windmill",0)});
    house->meshes.at(0).textures.push_back({TextureStore::instance()->loadTexture("../res/windmill_normal.tga", "windmill_norm",2)});
    Node *blades = windmill.getNode("Pales");
    blades->meshes.at(0).textures.clear();
    blades->meshes.at(0).textures.push_back({TextureStore::instance()->getTexture("windmill")});
    blades->meshes.at(0).textures.push_back({TextureStore::instance()->getTexture("windmill_norm")});

    vec3 windmillPos = terrain.root->meshes.at(0).vertices.at(mRandom(terrain.root->meshes.at(0).vertices.size())).Position;
    while (windmillPos.y < waterHeight) {
        windmillPos = terrain.root->meshes.at(0).vertices.at(mRandom(terrain.root->meshes.at(0).vertices.size())).Position;
    }
    windmillPos.y += 6.7;
    house->translate(windmillPos);
    return windmill;
}


void Terrain::plantGrassAt(vec3 position) {
    double x = position.x;
    double y = position.y;
    double z = position.z;
    unsigned int grassOffset = grassVerts.size();
    double rot = mRandom(2*M_PI); // random rotation of the blade
    grassVerts.push_back({vec3(x,y,z),vec3(0.0,0.0,1.0),vec2(0.0,0.0),vec3(sin(rot),0,cos(rot))});
    grassVerts.push_back({vec3(x+0.05*cos(rot),y,z+0.05*sin(rot)),vec3(0.0,0.0,1.0),vec2(1.0,0.0),vec3(sin(rot),0,cos(rot))});
    grassVerts.push_back({vec3(x+0.05*cos(rot),y+0.5,z+0.05*sin(rot)),vec3(0.0,0.0,1.0),vec2(1.0,0.5),vec3(sin(rot),0,cos(rot))});
    grassVerts.push_back({vec3(x,y+0.5,z),vec3(0.0,0.0,1.0),vec2(0.0,0.5),vec3(sin(rot),0,cos(rot))});
    grassVerts.push_back({vec3(x+0.025*cos(rot),y+1.0,z+0.025*cos(rot)),vec3(0.0,0.0,1.0),vec2(0.5,1.0),vec3(sin(rot),0,cos(rot))});
    grassIndices.push_back(0+grassOffset);
    grassIndices.push_back(1+grassOffset);
    grassIndices.push_back(2+grassOffset);
    grassIndices.push_back(0+grassOffset);
    grassIndices.push_back(2+grassOffset);
    grassIndices.push_back(3+grassOffset);
    grassIndices.push_back(3+grassOffset);
    grassIndices.push_back(2+grassOffset);
    grassIndices.push_back(4+grassOffset);
}

void Terrain::createGrassMesh() {
    Material mat = {vec3(0.2,0.2,0.2),
                    vec3(0.8,0.8,0.8),
                    vec3(0.6,0.6,0.6),
                    2};
    Texture tex = {TextureStore::instance()->getTexture("blade")};
    Texture normalMap = {TextureStore::instance()->getTexture("normalMap")};
    grass.root->meshes.push_back(Mesh(grassVerts,grassIndices,mat,tex, normalMap));
}
void Terrain::Draw(Shader shader, Shader waterShader) {
    terrain.Draw(shader);
    grass.Draw(shader);
    water.Draw(waterShader);
    windmill.Draw(shader);
}

