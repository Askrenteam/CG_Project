#include "Terrain.h"

Terrain::Terrain(int iterations, float alpha, double maxHeight)
{
    this->model = Model();
    model.root->meshes.push_back(generateTerrain(iterations, alpha, maxHeight));
//    generateTerrain(iterations, alpha);
}

// returns a random float between 0 and a
float mRandom(float a) {
    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/a));
}

Mesh Terrain::generateTerrain(int n, double alpha, double maxHeight) {
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
//        cout<<"After Diamond i="<<i<<endl;
//        for (int x=0; x<size; x++) {
//            for (int y = 0; y < size; y++) {
//                cout << heightMap[x][y] << "\t";
//            }
//            cout<<endl;
//        }

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
                                   heightMap[x][y+step] +
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
                                   heightMap[x][y+step] +
                                   heightMap[x][y+step]) / 4;
            }
            heightMap[size-1][y] = ((mRandom(2*alpha))-alpha+
                                   heightMap[size-1][y-step] +
                                    heightMap[size-1][y+step] +
                                    heightMap[size-1-step][y] ) / 3;
        }
//        cout<<"After Square i="<<i<<endl;
//        for (int x=0; x<size; x++) {
//            for (int y = 0; y < size; y++) {
//                cout << heightMap[x][y] << "\t";
//            }
//            cout<<endl;
//        }
        alpha *= 0.5;
    }

    Material mat = {vec3(0.4,0.4,0.4),
                    vec3(0.8,0.8,0.8),
                    vec3(0.0,0.0,0.0),
                    0.0};
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    unsigned int currVert = 0;


    for (int x=0; x<size; x++) {
        for (int y=0; y<size; y++) {
            Vertex vert;
            vert.Position = vec3(x,heightMap[x][y],y);
            vec3 n1 = vec3(0,0,0);
            vec3 n2 = vec3(0,0,0);
            vec3 n3 = vec3(0,0,0);
            vec3 n4 = vec3(0,0,0);
            if (x != 0) {
                float a = heightMap[x][y] - heightMap[x-1][y];
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
                float c = heightMap[x][y] - heightMap[x][y-1];
                n3 = vec3(0,1/sqrt(c*c+1),c/sqrt(c*c+1));
            }
            if (y != size-1) {
                float d = heightMap[x][y] - heightMap[x][y+1];
                n4 = vec3(0,1/sqrt(d*d+1),d/sqrt(d*d+1));
            }
            vert.Normal = normalize(n1+n2+n3+n4);
            vertices.push_back(vert);
            currVert++;
        }
//        cout<<endl;
    }

    return Mesh(vertices, indices, mat);

}
