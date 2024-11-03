#include "Terrain.h"

using namespace std;

float Terrain::shapeFunction(float x) {
    return 300.0f + 100.0f * sin(0.004f * x) + 60.0f * sin(0.02f * x) + 30.0f * sin(0.05f * x);
}

void Terrain::generateTerrain() {
    float flatness = 1;
    for (float x = 0; x < window->GetResolution().x; x += flatness)
    {
        float A_y = shapeFunction(x);
        float B_y = shapeFunction(x + flatness);
        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, A_y);
        modelMatrix *= transform2D::Shear(0, (B_y - A_y) / flatness);
        modelMatrix *= transform2D::Scale(flatness, -MAX(B_y, A_y));

        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
}