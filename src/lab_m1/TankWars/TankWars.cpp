#include "lab_m1/TankWars/TankWars.h"

#include <vector>
#include <iostream>
#include "lab_m1/TankWars/obj2D.h"
#include <vector>

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


void TankWars::MeshCreator() {
    // For terrain
    glm::vec3 corner = glm::vec3(0, 0, 0);
    Mesh* squareTerrain = obj2D::CreateSquare("squareTerrain", corner, 1, glm::vec3(0.796, 0.808, 0.569), true);
    AddMeshToList(squareTerrain);

    // For trajectory
    int side = 6;
    Mesh* square1Trajectory = obj2D::CreateSquare("square1Trajectory", glm::vec3(-side / 2, -side / 2, 0), side, glm::vec3(0.871, 0.192, 0.388), true);
    AddMeshToList(square1Trajectory);

    side = 4;
    Mesh* square2Trajectory = obj2D::CreateSquare("square2Trajectory", glm::vec3(-side / 2, -side / 2, 0), side, glm::vec3(0.871, 0.192, 0.388), true);
    AddMeshToList(square2Trajectory);

    side = 2;
    Mesh* square3Trajectory = obj2D::CreateSquare("square3Trajectory", glm::vec3(-side / 2, -side / 2, 0), side, glm::vec3(0.871, 0.192, 0.388), true);
    AddMeshToList(square3Trajectory);

    // For bomb
    side = 10;
    Mesh* squareBomb = obj2D::CreateSquare("squareBomb", glm::vec3(-side / 2, -side / 2, 0), side, glm::vec3(0.0, 1, 1), true);
    AddMeshToList(squareBomb);

    // For tank
    // Bottom trapez
    float topLength = 45;
    float bottomLength = 30;
    float height = 10;
    glm::vec3 center = glm::vec3(-bottomLength / 2, 0, 0);
    Mesh* trapezBottom = obj2D::CreateTrapezoid("trapezBottom", center, topLength, bottomLength, height, glm::vec3(0.133, 0.125, 0.137), true);
    AddMeshToList(trapezBottom);

    // Top trapez
    topLength = 50;
    bottomLength = 65;
    height = 15;
    center = glm::vec3(-bottomLength / 2, 10, 0);
    Mesh* trapezTop = obj2D::CreateTrapezoid("trapezTop", center, topLength, bottomLength, height, glm::vec3(0.66, 0.66, 0.66), true);
    AddMeshToList(trapezTop);

    // Turela
    center = glm::vec3(0, 25, 0);
    int radius = 10;
    Mesh* circleTurela = obj2D::CreateCircle("circleTurela", center, radius, glm::vec3(0.66, 0.66, 0.66), 49, true);
    AddMeshToList(circleTurela);

    // squareTankGun
    side = 5;
    Mesh* squareTankGun = obj2D::CreateSquare("squareTankGun", glm::vec3(-side / 2, 1.5, 0), side, glm::vec3(0.133, 0.125, 0.137), true);
    AddMeshToList(squareTankGun);
}

void TankWars::Init()
{
    MeshCreator();

    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    flatness = 1;
    terrain = new Terrain(flatness, resolution.x);
    heightMap = terrain->getHeightMap();
    
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


void TankWars::Update(float deltaTimeSeconds)
{
    // tankul
    
    
    int A_x = tankx - tankx % flatness;
    int B_x = A_x + flatness;
    float A_y = heightMap[A_x];
    float B_y = heightMap[B_x];
    float t = (tankx - A_x) / (B_x - A_x);
    tanky = A_y + t * (B_y - A_y);

    rotationAngle = atan2((B_y - A_y) , (B_x - A_x));

    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(A_x, tanky);
    modelMatrix *= transform2D::Rotate(rotationAngle);

    RenderMesh2D(meshes["trapezBottom"], shaders["VertexColor"], modelMatrix);

    RenderMesh2D(meshes["trapezTop"], shaders["VertexColor"], modelMatrix);

    RenderMesh2D(meshes["circleTurela"], shaders["VertexColor"], modelMatrix);

    // squareTankGun
    modelMatrix *= transform2D::Translate(0, 25);
    modelMatrix *= transform2D::Rotate(angleOfAttack - rotationAngle);
    modelMatrix *= transform2D::Scale(1, 10 / 1.5);
    RenderMesh2D(meshes["squareTankGun"], shaders["VertexColor"], modelMatrix);


    // Traiectorie
    glm::vec2 position = glm::vec2(tankx, tanky);
    float gravity = 9.81f;
    
    if (shooting) {
        modelMatrix = glm::mat3(1.0f);
        modelMatrix *= transform2D::Translate(posBombaX, posBombaY);
        modelMatrix *= transform2D::Rotate(rotationAngle);
        modelMatrix *= transform2D::Translate(0, 25);
        modelMatrix *= transform2D::Rotate(TrajRotation);
        RenderMesh2D(meshes["squareBomb"], shaders["VertexColor"], modelMatrix);

        TrajRotation += 0.1;
        staticDT = deltaTimeSeconds * 10;
        posBombaX += v.x * staticDT;
        posBombaY += v.y * staticDT;
        v.y -= gravity * staticDT;
        if (posBombaY < heightMap[int(posBombaX)] - 25 || posBombaX > window->GetResolution().x) {
            shooting = false;
            v.y += gravity * staticDT;
            posBombaX -= v.x * staticDT;
            posBombaY -= v.y * staticDT;
            int radius = 50;
            for (size_t i = 0; i < 50; i++) {
                float leftSide = heightMap[int(posBombaX)] - glm::sqrt(radius * radius - (i - 50) * (i - 50));
                float rightSide = heightMap[int(posBombaX)] - glm::sqrt(radius * radius - (49 - i) * (49 - i));
                if (heightMap[int(posBombaX) - 50 + i] > leftSide) {
                    heightMap[int(posBombaX) - 50 + i] = leftSide;
                }
                if (heightMap[int(posBombaX) + 49 - i] > rightSide)
                    heightMap[int(posBombaX) + 49 - i] = rightSide;
            }
            TrajRotation = 0.017;
        }
    }
    
    staticDT = 0.3;
    glm::vec2 v2 = glm::vec2(power * glm::cos(angleOfAttack + glm::pi<float>() / 2),
        power * glm::sin(angleOfAttack + glm::pi<float>() / 2));
    for (float t = 0.0f; t < 6.0f; t += staticDT) {
        modelMatrix = glm::mat3(1.0f);
        modelMatrix *= transform2D::Translate(position.x, position.y);
        modelMatrix *= transform2D::Rotate(rotationAngle);
        modelMatrix *= transform2D::Translate(0, 25);
        if (t < 3) {
            RenderMesh2D(meshes["square1Trajectory"], shaders["VertexColor"], modelMatrix);
        }
        else if (t < 4.5) {
            RenderMesh2D(meshes["square2Trajectory"], shaders["VertexColor"], modelMatrix);
        }
        else {
            RenderMesh2D(meshes["square3Trajectory"], shaders["VertexColor"], modelMatrix);
        }

        position.x += v2.x * staticDT;
        position.y += v2.y * staticDT;
        v2.y -= gravity * staticDT;

        if (position.y < heightMap[int(position.x)] - 25 || position.x > window->GetResolution().x) break;
    }

    // Default (0, 0)
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Rotate(rotationAngle);
    modelMatrix *= transform2D::Translate(0, 25);
    modelMatrix *= transform2D::Rotate(angleOfAttack);
    modelMatrix *= transform2D::Scale(1, 5);
    RenderMesh2D(meshes["squareTankGun"], shaders["VertexColor"], modelMatrix);
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["trapezBottom"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["trapezTop"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["circleTurela"], shaders["VertexColor"], modelMatrix);


    // Terrain Generate
    vector<glm::mat3> heightMapModels = terrain->heightMapModelGenerator();
    for (glm::mat3 model : heightMapModels) {
        RenderMesh2D(meshes["squareTerrain"], shaders["VertexColor"], model);
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
        tankx += 80 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        tankx -= 40 * deltaTime ;
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        angleOfAttack += 0.66 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        angleOfAttack -= 0.66 * deltaTime;
    }
    /*if (window->KeyHold(GLFW_KEY_SPACE)) {
        power += 1;
    }
    if (window->KeyHold(GLFW_KEY_U)) {
        power -= 1;
    }*/
}


void TankWars::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE) {
        if (!shooting) {
            shooting = true;
            v = glm::vec2(power * glm::cos(angleOfAttack + glm::pi<float>() / 2),
                power * glm::sin(angleOfAttack + glm::pi<float>() / 2));
            posBombaX = tankx;
            posBombaY = tanky;
        }
    }
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
