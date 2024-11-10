#include "Terrain.h"
#include <lab_m1/TankWars/transform2D.h>
#include "components/simple_scene.h"
#include "lab_m1/TankWars/obj2D.h"

using namespace std;

float Terrain::shapeFunction(float x) {
    return 300.0f + 100.0f * sin(0.004f * x) + 60.0f * sin(0.02f * x) + 30.0f * sin(0.05f * x);
}

float *Terrain::heightMapGenerator() {
    float flatness = 1;
    float heightMap[1280];
    glm::vec3 corner = glm::vec3(0, 0, 0);
    for (int x = 0; x < 1280; x += flatness) {
        float y = shapeFunction(x);
        heightMap[x] = y;
    }
    return heightMap;
}