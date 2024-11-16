#include "Terrain.h"
#include <lab_m1/TankWars/transform2D.h>
#include "lab_m1/TankWars/obj2D.h"
#include <vector>

using namespace std;

Terrain::Terrain(int flatness, int width) {
    this->flatness = flatness;
    this->width = width;
    this->heightMap = this->heightMapGenerator();
}
float Terrain::shapeFunction(float x) {
    return 250.0f + 100.0f * sin(0.004f * x) + 60.0f * sin(0.02f * x) + 30.0f * sin(0.05f * x);
}
float* Terrain::heightMapGenerator() {
    float* heightMap = new float[width];
    for (int x = 0; x < width; x += flatness) {
        float y = shapeFunction(x);
        heightMap[x] = y;
    }
    return heightMap;
}
vector<glm::mat3> Terrain::heightMapModelGenerator() {
    vector<glm::mat3> models;

    for (int x = 0; x < width; x += flatness)
    {
        float A_y = heightMap[x];
        float B_y = heightMap[x + 1];

        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, A_y);
        modelMatrix *= transform2D::Shear(0, (B_y - A_y) / flatness);
        modelMatrix *= transform2D::Scale(flatness, -MAX(B_y, A_y));
        models.push_back(modelMatrix);

    }
    return models;
}

float* Terrain::getHeightMap() {
    return heightMap;
}
int Terrain::getFlatness() {
    return flatness;
}
int Terrain::getWidth() {
    return width;
}