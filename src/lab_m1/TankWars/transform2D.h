#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::transpose(
            glm::mat3(1, 0, translateX,
                0, 1, translateY,
                0, 0, 1)
        );

    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::transpose(
            glm::mat3(scaleX, 0, 0,
                0, scaleY, 0,
                0, 0, 1)
        );

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        return glm::transpose(
            glm::mat3(glm::cos(radians), -glm::sin(radians), 0,
                glm::sin(radians), glm::cos(radians), 0,
                0, 0, 1)
        );

    }

    // Shear Matrix
    inline glm::mat3 Shear(float shearX, float shearY)
    {
        return glm::transpose(
            glm::mat3(1, shearX, 0,
                shearY, 1, 0,
                0, 0, 1)
        );
    }
}   // namespace transform2D
