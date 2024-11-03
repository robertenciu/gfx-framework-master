#include "lab_m1/TankWars/TankWars.h"

#include <vector>
#include <iostream>
#include "lab_m1/TankWars/obj2D.h"
#include "lab_m1/TankWars/Terrain.h"
using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


TankWars::TankWars()
{

}


TankWars::~TankWars()
{
}



void TankWars::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 1;

    float topLength = 45;
    float bottomLength = 30;
    float height = 10;
    glm::vec3 center = glm::vec3(-bottomLength / 2, 0, 0);
    Mesh* square1 = obj2D::CreateSquare("square1", corner, 1, glm::vec3(0.796, 0.808, 0.569), true);
    AddMeshToList(square1);

    Mesh* trapez1 = obj2D::CreateTrapezoid("trapez1", center, 45, 30, 10, glm::vec3(0.133, 0.125, 0.137), true);
    AddMeshToList(trapez1);

    topLength = 50;
    bottomLength = 65;
    height = 15;
    center = glm::vec3(-bottomLength / 2, 10, 0);
    Mesh* trapez2 = obj2D::CreateTrapezoid("trapez2", center, 50, 65, 15, glm::vec3(0.66, 0.66, 0.66), true);
    AddMeshToList(trapez2);

    center = glm::vec3(0, 25, 0);
    Mesh* circle1 = obj2D::CreateCircle("circle1", center, 10, glm::vec3(0.66, 0.66, 0.66), 49, true);
    AddMeshToList(circle1);
}


void TankWars::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.463, 0.322, 0.545, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

//float terrainFunction(float x) {
//    return 255.0f + 30.0f * sin(0.015f * x) + 10.f * sin(0.042f * x) + 30.0f * sin(0.008f * x);
//}
//float terrainFunction(float x) {
//    return 150.0f + 70.0f * sin(0.008f * x) + 40.0f * sin(0.015f * x) + 20.0f * sin(0.03f * x);
//}

float shapeFunction(float x) {
    return 250.0f + 100.0f * sin(0.004f * x) + 60.0f * sin(0.02f * x) + 30.0f * sin(0.05f * x);
}
void TankWars::Update(float deltaTimeSeconds)
{

    float flatness = 1;

    // tankul
    glm::mat3 modelMatrix = glm::mat3(1);
    float A_x = tankx - tankx % int(flatness);
    float B_x = A_x + flatness;
    float A_y = shapeFunction(A_x);
    float B_y = shapeFunction(A_x + flatness);
    float t = (tankx - A_x) / (B_x - A_x);
    int tanky = A_y + t * (B_y - A_y);
    float rotationAngle = atan2((B_y - A_y) , (B_x - A_x));
    modelMatrix *= transform2D::Translate(tankx, tanky);
    modelMatrix *= transform2D::Rotate(rotationAngle);
    RenderMesh2D(meshes["trapez1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);

    modelMatrix *= transform2D::Translate(tankx, tanky);
    modelMatrix *= transform2D::Rotate(rotationAngle);
    RenderMesh2D(meshes["trapez2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tankx, tanky);
    modelMatrix *= transform2D::Rotate(rotationAngle);
    RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrix);

    // terenu boss
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


void TankWars::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_D)) {
        tankx += 100 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        tankx -= 50 * deltaTime ;
    }
}


void TankWars::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void TankWars::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void TankWars::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void TankWars::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void TankWars::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void TankWars::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void TankWars::OnWindowResize(int width, int height)
{
}
