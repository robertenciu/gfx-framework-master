#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace obj2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateTrapez(const std::string& name,
        glm::vec3 leftBottomCorner,
        float topLength,
        float bottomLength,
        float height,
        glm::vec3 color,
        bool fill);
    Mesh* CreateCircle(
        const std::string& name,
        glm::vec3 center,
        float radius,
        glm::vec3 color,
        int numSegments,
        bool fill);
}