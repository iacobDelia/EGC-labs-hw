#include "object3d.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object3d::CreateSquare(
    const std::string& name,
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
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0, 2 };

    square->SetDrawMode(GL_TRIANGLES);

    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object3d::CreatePara(
    const std::string& name,
    glm::vec3 center,
    float height,
    float width,
    float length,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center, color), // 0
        VertexFormat(center + glm::vec3(-width / 2, 0, -length / 2), color), // 1
        VertexFormat(center + glm::vec3(width / 2, 0, -length / 2), color), // 2
        VertexFormat(center + glm::vec3(width / 2, 0, length / 2), color), // 3
        VertexFormat(center + glm::vec3(-width / 2, 0, length / 2), color), // 4

        VertexFormat(center + glm::vec3(-width / 2, height, -length / 2), color), // 5
        VertexFormat(center + glm::vec3(width / 2, height, -length / 2), color), // 6
        VertexFormat(center + glm::vec3(width / 2, height, length / 2), color), // 7
        VertexFormat(center + glm::vec3(-width / 2, height, length / 2), color), // 8

    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0, 2, 3, 4, 0, 4, 1, 0, // partea de jos
                                            1, 5, 2, 2, 5, 6, // partea din spate
                                            3, 6, 7, 3, 2, 6, // partea din dreapta
                                            4, 8, 5, 1, 4, 5, // partea din stanga
                                            4, 3, 8, 3, 7, 8, // partea din fata
                                            7, 5, 6, 7, 6, 5 // capacul
    };

    square->SetDrawMode(GL_TRIANGLES);

    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object3d::CreateTerrain(const std::string& name, int vertCount, glm::vec3 botLeftCorner, float cellWidth, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 crtPlace = glm::vec3(0, 0, 0);
    // first generate all vertices
    vertices.push_back(glm::vec3(0, 0, 0));
    for (int i = 1; i <= vertCount; i++) {
        for (int j = 1; j <= vertCount; j++) {
            vertices.push_back(VertexFormat(crtPlace, color));
            crtPlace.z += cellWidth;
        }
        crtPlace.z = 0;
        crtPlace.x += cellWidth;
    }
    // traverse vertically

    for(int i = 1; i <= vertCount - 1; i++) {
        // traverse horizontally
        // add the current row
        int startInd = (i - 1) * vertCount + 1;
        int startIndNextRow = i * vertCount + 1;
        for (int j = 0; j < vertCount; j++) {
            indices.push_back(startInd + j);
            indices.push_back(startIndNextRow + j);
        }
        // add these again to create "fake" triangles
        indices.push_back(startIndNextRow - 1 + vertCount);
        indices.push_back(startIndNextRow);
    }

    Mesh* terrain = new Mesh(name);
    terrain->SetDrawMode(GL_TRIANGLE_STRIP);
    terrain->InitFromData(vertices, indices);
    return terrain;
    
}
Mesh* object3d::CreateCylinder(const std::string& name, glm::vec3 center, float height, float radius, glm::vec3 color) {

    glm::vec3 lowP = glm::vec3((sqrt(3) / 2) * radius, 0, 0.5 * radius);
    glm::vec3 highP = glm::vec3(0.5 * radius, 0, (sqrt(3)/2) * radius);
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(radius, 0, 0), color), // 0
        VertexFormat(glm::vec3(radius, height, 0), color), // 1

        VertexFormat(glm::vec3(lowP.x, 0, -lowP.z), color), // 2
        VertexFormat(glm::vec3(lowP.x, height, -lowP.z), color), // 3

        VertexFormat(glm::vec3(highP.x, 0, -highP.z), color), // 4
        VertexFormat(glm::vec3(highP.x, height, -highP.z), color), // 5

        VertexFormat(glm::vec3(0, 0, -radius), color), // 6
        VertexFormat(glm::vec3(0, height, -radius), color), // 7

        VertexFormat(glm::vec3(-highP.x, 0, -highP.z), color), // 8
        VertexFormat(glm::vec3(-highP.x, height, -highP.z), color), // 9

        VertexFormat(glm::vec3(-lowP.x, 0, -lowP.z), color), // 10
        VertexFormat(glm::vec3(-lowP.x, height, -lowP.z), color), // 11

        VertexFormat(glm::vec3(-radius, 0, 0), color), // 12
        VertexFormat(glm::vec3(-radius, height, 0), color), // 13

        VertexFormat(glm::vec3(-lowP.x, 0, lowP.z), color),// 14
        VertexFormat(glm::vec3(-lowP.x, height, lowP.z), color),// 15

        VertexFormat(glm::vec3(-highP.x, 0, highP.z), color),// 16
        VertexFormat(glm::vec3(-highP.x, height, highP.z), color),// 17

        VertexFormat(glm::vec3(0, 0, radius), color),// 18
        VertexFormat(glm::vec3(0, height, radius), color),// 19

        VertexFormat(glm::vec3(highP.x, 0, highP.z), color),// 20
        VertexFormat(glm::vec3(highP.x, height, highP.z), color),// 21

        VertexFormat(glm::vec3(lowP.x, 0, lowP.z), color),// 22
        VertexFormat(glm::vec3(lowP.x, height, lowP.z), color),// 23

    };

    std::vector<unsigned int> indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                         11, 12, 13, 14, 15, 16, 17, 18, 19,
                                            20, 21, 22, 23, 0, 1};
    Mesh* cylinder = new Mesh(name);
    cylinder->SetDrawMode(GL_TRIANGLE_STRIP);
    cylinder->InitFromData(vertices, indices);
    return cylinder;
}
Mesh* object3d::CreateCone(const std::string& name, glm::vec3 center, float height, float radius, glm::vec3 color) {
    
    glm::vec3 lowP = glm::vec3((sqrt(3) / 2) * radius, 0, 0.5 * radius);
    glm::vec3 highP = glm::vec3(0.5 * radius, 0, (sqrt(3) / 2) * radius);
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, height, 0), color), // 0

        VertexFormat(glm::vec3(radius, 0, 0), color), // 1

        VertexFormat(glm::vec3(lowP.x, 0, -lowP.z), color), // 2

        VertexFormat(glm::vec3(highP.x, 0, -highP.z), color), // 3

        VertexFormat(glm::vec3(0, 0, -radius), color), // 4

        VertexFormat(glm::vec3(-highP.x, 0, -highP.z), color), // 5

        VertexFormat(glm::vec3(-lowP.x, 0, -lowP.z), color), // 6

        VertexFormat(glm::vec3(-radius, 0, 0), color), // 7

        VertexFormat(glm::vec3(-lowP.x, 0, lowP.z), color),// 8

        VertexFormat(glm::vec3(-highP.x, 0, highP.z), color),// 9

        VertexFormat(glm::vec3(0, 0, radius), color),// 10

        VertexFormat(glm::vec3(highP.x, 0, highP.z), color),// 11

        VertexFormat(glm::vec3(lowP.x, 0, lowP.z), color),// 12

    };

    std::vector<unsigned int> indices = { 1, 0, 2, 0, 3, 0, 4, 0, 5, 0,
                                            6, 0, 7, 0, 8, 0, 9, 0, 
                                            10, 0, 11, 0, 12, 0, 1};
    Mesh* cylinder = new Mesh(name);
    cylinder->SetDrawMode(GL_TRIANGLE_STRIP);
    cylinder->InitFromData(vertices, indices);
    return cylinder;
}
Mesh* object3d::CreateArrow(const std::string& name, glm::vec3 center, float len, glm::vec3 color) {
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(-0.5 * len, 0, 0) , color), // 0
        VertexFormat(center + glm::vec3(0, 0, len) , color), // 1
        VertexFormat(center + glm::vec3(0.5 * len, 0, 0) , color), // 2
    };
    std::vector<unsigned int> indices = { 0, 1, 2 };
    Mesh* arrow = new Mesh(name);
    arrow->SetDrawMode(GL_LINE_STRIP);
    arrow->InitFromData(vertices, indices);
    glLineWidth(3);
    return arrow;
}
Mesh* object3d::CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color) {

    glm::vec3 lowP = glm::vec3((sqrt(3) / 2) * radius, 0, 0.5 * radius);
    glm::vec3 highP = glm::vec3(0.5 * radius, 0, (sqrt(3) / 2) * radius);
    std::vector<VertexFormat> vertices =
    {

        VertexFormat(glm::vec3(radius, 0, 0), color), // 1

        VertexFormat(glm::vec3(lowP.x, 0, -lowP.z), color), // 2

        VertexFormat(glm::vec3(highP.x, 0, -highP.z), color), // 3

        VertexFormat(glm::vec3(0, 0, -radius), color), // 4

        VertexFormat(glm::vec3(-highP.x, 0, -highP.z), color), // 5

        VertexFormat(glm::vec3(-lowP.x, 0, -lowP.z), color), // 6

        VertexFormat(glm::vec3(-radius, 0, 0), color), // 7

        VertexFormat(glm::vec3(-lowP.x, 0, lowP.z), color),// 8

        VertexFormat(glm::vec3(-highP.x, 0, highP.z), color),// 9

        VertexFormat(glm::vec3(0, 0, radius), color),// 10

        VertexFormat(glm::vec3(highP.x, 0, highP.z), color),// 11

        VertexFormat(glm::vec3(lowP.x, 0, lowP.z), color),// 12

    };

    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0 };
    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_LINE_STRIP);
    circle->InitFromData(vertices, indices);
    return circle;
}