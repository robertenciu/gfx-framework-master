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

Mesh* obj2D::CreateTrapezoid(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float topLength,
    float bottomLength,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    glm::vec3 bottomLeft = corner;
    glm::vec3 bottomRight = corner + glm::vec3(bottomLength, 0, 0);
    glm::vec3 topLeft = corner + glm::vec3((bottomLength - topLength) / 2, height, 0);
    glm::vec3 topRight = corner + glm::vec3((bottomLength + topLength) / 2, height, 0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(bottomLeft, color),
        VertexFormat(bottomRight, color),
        VertexFormat(topRight, color),
        VertexFormat(topLeft, color)
    };

    Mesh* trapezoid = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        trapezoid->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the indices to form the trapezoid
        indices.push_back(0); // back to bottom left for triangle 1
        indices.push_back(2); // to top right for triangle 1
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

    // Add the center vertex
    vertices.push_back(VertexFormat(center, color));

    // Calculate the circle points and add vertices
    for (int i = 0; i <= numSegments; i++) {
        float angle = i * 2.0f * glm::pi<float>() / numSegments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);

        vertices.push_back(VertexFormat(glm::vec3(x, y, center.z), color));
        indices.push_back(i + 1);
    }

    // Close the circle
    indices.push_back(1);

    Mesh* circle = new Mesh(name);

    if (fill) {
        // Draw triangles from the center to each pair of perimeter vertices
        for (int i = 1; i < numSegments; i++) {
            indices.push_back(0);        // Center
            indices.push_back(i);        // Current perimeter vertex
            indices.push_back(i + 1);    // Next perimeter vertex

        }
        indices.push_back(0);
        indices.push_back(numSegments);
        indices.push_back(1);
    }
    else {
        // Use line loop mode for an unfilled circle outline
        circle->SetDrawMode(GL_LINE_LOOP);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

