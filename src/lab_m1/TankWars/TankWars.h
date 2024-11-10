#pragma once

#include "components/simple_scene.h"
#include "lab_m1/TankWars/Terrain.h"


namespace m1
{
    class TankWars : public gfxc::SimpleScene
    {
    public:
        TankWars();
        ~TankWars();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        int tankx = 400;
        float heightMap[1280];
        int flatness;
        float angleOfAttack = 0;
        float staticDT = 0.0145815;
        float power = 60;
        bool shooting = false;
        float posBombaX = 0;
        float posBombaY = 0;
        glm::vec2 v;
        float rotationAngle = 0;
        float tanky;
    };
}   // namespace m1
