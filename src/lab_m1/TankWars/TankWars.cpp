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
    Mesh* square1Trajectory = obj2D::CreateSquare("square1Trajectory", glm::vec3(-side / 2, -side / 2, 0), side, glm::vec3(1, 1, 1), true);
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

    corner = glm::vec3(-2.5, 10, 0);
    Mesh* health = obj2D::CreateSquare("health", corner, 5, glm::vec3(0.796, 0.808, 0.569), true);
    AddMeshToList(health);
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


    terrain = new Terrain(1, resolution.x);
    
    firstTank = new Tank(300, terrain);
    secondTank = new Tank(700, terrain);
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

void TankWars::Update(float deltaTimeSeconds)
{
    // First tank
    if (firstTank->getHealth() > 0) {
        firstTank->update(deltaTimeSeconds);

        glm::mat3 firstTankBaseModel = firstTank->getBaseModel();
        RenderMesh2D(meshes["trapezBottom"], shaders["VertexColor"], firstTankBaseModel);
        RenderMesh2D(meshes["trapezTop"], shaders["VertexColor"], firstTankBaseModel);
        RenderMesh2D(meshes["circleTurela"], shaders["VertexColor"], firstTankBaseModel);

        glm::mat3 firstTankGunModel = firstTank->getGunModel();
        RenderMesh2D(meshes["squareTankGun"], shaders["VertexColor"], firstTankGunModel);

        vector<glm::mat3> firstTankTrajectoryModels = firstTank->trajectoryModelGenerator();
        for (glm::mat3 model : firstTankTrajectoryModels) {
            RenderMesh2D(meshes["square1Trajectory"], shaders["VertexColor"], model);
        }

        glm::mat3 firstTankHealthModel = firstTank->getHealthModel();
        RenderMesh2D(meshes["health"], shaders["VertexColor"], firstTankHealthModel);

        firstTank->updateShooting(secondTank);
        if (firstTank->isTankShooting()) {
            RenderMesh2D(meshes["squareBomb"], shaders["VertexColor"], firstTank->getBombModel());
        }
    }
    // Second tank
    if (secondTank->getHealth() > 0) {
        secondTank->update(deltaTimeSeconds);

        glm::mat3 secondTankBaseModel = secondTank->getBaseModel();
        RenderMesh2D(meshes["trapezBottom"], shaders["VertexColor"], secondTankBaseModel);
        RenderMesh2D(meshes["trapezTop"], shaders["VertexColor"], secondTankBaseModel);
        RenderMesh2D(meshes["circleTurela"], shaders["VertexColor"], secondTankBaseModel);

        glm::mat3 secondTankGunModel = secondTank->getGunModel();
        RenderMesh2D(meshes["squareTankGun"], shaders["VertexColor"], secondTankGunModel);

        vector<glm::mat3> secondTankTrajectoryModels = secondTank->trajectoryModelGenerator();
        for (glm::mat3 model : secondTankTrajectoryModels) {
            RenderMesh2D(meshes["square1Trajectory"], shaders["VertexColor"], model);
        }

        glm::mat3 secondTankHealthModel = secondTank->getHealthModel();
        RenderMesh2D(meshes["health"], shaders["VertexColor"], secondTankHealthModel);

        secondTank->updateShooting(firstTank);
        if (secondTank->isTankShooting()) {
            RenderMesh2D(meshes["squareBomb"], shaders["VertexColor"], secondTank->getBombModel());
        }
    }
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
        firstTank->moveRight();
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        firstTank->moveLeft();
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        firstTank->updateAngleOfAttack(true);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        firstTank->updateAngleOfAttack(false);
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        secondTank->moveRight();
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        secondTank->moveLeft();
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        secondTank->updateAngleOfAttack(false);
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        secondTank->updateAngleOfAttack(true);
    }
}


void TankWars::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE) {
        firstTank->startShooting();
    }
    if (key == GLFW_KEY_ENTER) {
        secondTank->startShooting();
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
