#include "obj2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* obj2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* obj2D::CreateTrapez(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float topLength,
    float bottomLength,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(bottomLength, 0, 0), color),
        VertexFormat(corner + glm::vec3((bottomLength + topLength) / 2, height, 0), color),
        VertexFormat(corner + glm::vec3((bottomLength - topLength) / 2, height, 0), color)
    };

    Mesh* trapezoid = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        trapezoid->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}
Mesh* obj2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    int numSegments,
    bool fill = true)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(VertexFormat(center, color));

    for (int i = 0; i <= numSegments; i++) {
        float angle = i * 2.0f * glm::pi<float>() / numSegments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);

        vertices.push_back(VertexFormat(glm::vec3(x, y, center.z), color));
        indices.push_back(i);
    }

    Mesh* circle = new Mesh(name);

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        for (int i = 1; i < numSegments; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);

        }
        indices.push_back(0);
        indices.push_back(numSegments);
        indices.push_back(1);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

