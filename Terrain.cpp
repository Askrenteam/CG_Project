#include "Terrain.h"
#include "TextureStore.h"

Terrain::Terrain(int iterations, float alpha, double resSize, double maxHeight)
{
    this->terrain = Model();
    this->grass = Model();
    terrain.root->meshes.push_back(generateTerrain(iterations, alpha, resSize,  maxHeight));
    terrain.root->meshes.push_back(generateWater(resSize,  maxHeight/3));
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
//        cout<<"step : "<<step<<endl;
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
                    vec3(0.0,0.0,0.0),
                    2};
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    unsigned int currVert = 0;

    for (int x=0; x<size; x++) {
        for (int y=0; y<size; y++) {
            Vertex vert;
            double resizeFactor = resSize/(float)size;
            vert.Position = vec3(x*resizeFactor,heightMap[x][y]*resizeFactor,y*resizeFactor);
            plantGrassAt(vec3(vert.Position.x+mRandom(resizeFactor)-resizeFactor/2,
                              vert.Position.y,
                              vert.Position.z+mRandom(resizeFactor)-resizeFactor/2));
//            cout << vert.Position.x<<" "<<vert.Position.y<<" "<<vert.Position.z<<" "<<endl;
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
            vertices.push_back(vert);
            currVert++;
        }
//        cout<<endl;
    }
    createGrassMesh();
    Texture tex = {TextureStore::instance()->getTexture("grass")};
    return Mesh(vertices, indices, mat, tex);

}

Mesh Terrain::generateWater(double size, double waterLevel) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    Material mat = {vec3(0.3,0.3,0.7),
                    vec3(0.1,0.1,0.7),
                    vec3(0.6,0.6,0.6),
                    2};

    vertices.push_back({vec3(0,waterLevel,0),vec3(0.0,1.0,0.0), vec2(0.0,size)});
    vertices.push_back({vec3(0,waterLevel,size-1),vec3(0.0,1.0,0.0), vec2(0.0,0.0)});
    vertices.push_back({vec3(size-1,waterLevel,0),vec3(0.0,1.0,0.0), vec2(size,size)});
    vertices.push_back({vec3(size-1,waterLevel,size-1),vec3(0.0,1.0,0.0), vec2(size,0.0)});
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(3);
    Texture tex = {TextureStore::instance()->getTexture("water")};
    return Mesh(vertices, indices, mat, tex);
}

void Terrain::plantGrassAt(vec3 position) {
    double x = position.x;
    double y = position.y;
    double z = position.z;
    unsigned int grassOffset = grassVerts.size();
    double rot = mRandom(2*M_PI); // random rotation of the blade
    grassVerts.push_back({vec3(x,y,z),vec3(0.0,0.0,1.0),vec2(0.0,0.0)});
    grassVerts.push_back({vec3(x+0.05*cos(rot),y,z+0.05*sin(rot)),vec3(0.0,0.0,1.0),vec2(1.0,0.0)});
    grassVerts.push_back({vec3(x+0.05*cos(rot),y+0.5,z+0.05*sin(rot)),vec3(0.0,0.0,1.0),vec2(1.0,0.5)});
    grassVerts.push_back({vec3(x,y+0.5,z),vec3(0.0,0.0,1.0),vec2(0.0,0.5)});
    grassVerts.push_back({vec3(x+0.025*cos(rot),y+1.0,z+0.025*cos(rot)),vec3(0.0,0.0,1.0),vec2(0.5,1.0)});
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
    Material mat = {vec3(0.3,0.5,0.3),
                    vec3(0.8,0.8,0.8),
                    vec3(0.6,0.6,0.6),
                    2};
    Texture tex = {TextureStore::instance()->getTexture("blade")};
    grass.root->meshes.push_back(Mesh(grassVerts,grassIndices,mat,tex));
}
void Terrain::Draw(Shader shader) {
//    shader.setUniformMat4("proj", perspective(45.0f, (float)800 / (float)600, 0.1f, 1000.0f));
    terrain.Draw(shader);
//    shader.setUniformMat4("proj", perspective(45.0f, (float)800 / (float)600, 0.1f, 1.0f));
    grass.Draw(shader);
}
