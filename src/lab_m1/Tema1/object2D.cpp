#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        /*VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)*/
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, -length, 0), color),
        VertexFormat(corner + glm::vec3(length, -length, 0), color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        
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

Mesh* object2D::CreateTank(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        //VertexFormat(corner, color),
        //VertexFormat(corner + glm::vec3(0, -length, 0), color),
        //VertexFormat(corner + glm::vec3(length, -length, 0), color),
        //VertexFormat(corner + glm::vec3(length, 0, 0), color),

        //VertexFormat(corner, color),
        //VertexFormat(corner + glm::vec3(0.7 * length, 0, 0), color),
        //VertexFormat(corner + glm::vec3(length, 0.5 * length, 0), glm::vec3(0.5, 1, 0)),
        //
        //VertexFormat(corner + glm::vec3(-1 * length, 0.5 * length, 0), color),
        //VertexFormat(corner + glm::vec3(-0.7 * length, 0, 0), color),

        VertexFormat(corner, color), // 0
        VertexFormat(corner + glm::vec3(-0.5 * length, 0.15 * length, 0), color), // 1
        VertexFormat(corner + glm::vec3(-0.4 * length, 0, 0), color), // 2
        VertexFormat(corner + glm::vec3(0.5 * length, 0.15 * length, 0), color), // 3
        VertexFormat(corner + glm::vec3(0.4 * length, 0, 0), color), // 4

        VertexFormat(corner + glm::vec3(0, 0.15 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 5
        VertexFormat(corner + glm::vec3(-0.5 * length, 0.4 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 6
        VertexFormat(corner + glm::vec3(-0.6 * length, 0.15 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 7
        VertexFormat(corner + glm::vec3(0.5 * length, 0.4 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 8
        VertexFormat(corner + glm::vec3(0.6 * length, 0.15 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 9

        VertexFormat(corner + glm::vec3(0, 0.4 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 10
        VertexFormat(corner + glm::vec3(-0.172 * length, 0.5 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 11
        VertexFormat(corner + glm::vec3(-0.2 * length, 0.4 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 12
        VertexFormat(corner + glm::vec3(-0.1 * length, 0.573 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 13
        VertexFormat(corner + glm::vec3(0, 0.6 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 14
        VertexFormat(corner + glm::vec3(0.1 * length, 0.573 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 15
        VertexFormat(corner + glm::vec3(0.172 * length, 0.5 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 16
        VertexFormat(corner + glm::vec3(0.2 * length, 0.4 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)) // 17
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 0, 3, 1, 0, 4, 3, // first trapeze
                                            5, 6, 7, 5, 8, 6, 5, 9 , 8, // second one
                                            10, 11, 12, 10, 13, 11, 10, 14, 13, 
                                            10, 15, 14, 10, 16, 15, 10, 17, 16}; // circle

    square->SetDrawMode(GL_TRIANGLES);
    

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCannon(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    corner += glm::vec3(0, 0, 0);
    //std::vector<VertexFormat> vertices =
    //{
    //    VertexFormat(corner + glm::vec3(0, -0.4 * length, 0), color), // 0
    //    VertexFormat(corner + glm::vec3(0, 0.3 * length, 0))

    //};

    //Mesh* square = new Mesh(name);
    //std::vector<unsigned int> indices = {0, 1};
    //square->SetDrawMode(GL_LINES);

    //square->InitFromData(vertices, indices);
    //return square;
    std::vector<VertexFormat> vertices = {
    VertexFormat(corner, color), // 0
    VertexFormat(corner + glm::vec3(-0.01 * length, 0.4 * length, 0), color), // 1
    VertexFormat(corner + glm::vec3(-0.01 * length, 0, 0), color), // 2
    VertexFormat(corner + glm::vec3(0.01 * length, 0.4 * length, 0), color), // 3
    VertexFormat(corner + glm::vec3(0.01 * length, 0, 0), color), // 4

    VertexFormat(corner + glm::vec3(0, 0.3 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 5
    VertexFormat(corner + glm::vec3(-0.017 * length, 0.45 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 6
    VertexFormat(corner + glm::vec3(-0.017 * length, 0.4 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 7
    VertexFormat(corner + glm::vec3(0.017 * length, 0.45 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 8
    VertexFormat(corner + glm::vec3(0.017 * length, 0.4 * length, 0), color + glm::vec3(0.2, 0.2, 0.2)), // 9



};

Mesh* square = new Mesh(name);
std::vector<unsigned int> indices = { 5, 6, 7, 5, 8, 6, 5, 9, 8, 0, 1, 2, 0, 3, 1, 0, 4, 3
                                        };
square->SetDrawMode(GL_TRIANGLES);
square->InitFromData(vertices, indices);
return square;
}

Mesh* object2D::CreateTrajectory(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec2 P = glm::vec2(corner.x + 10, corner.y + 10);
    glm::vec2 v = glm::vec2(cos(0.8) * 100, sin(0.8) * 100);
    glm::vec2 g = glm::vec2(0, 300);

    float unit = 0.01;
    
    for (int i = 0; i < 1000; i++) {
        VertexFormat vertex = VertexFormat(corner + glm::vec3(P.x, P.y, 0), color);
        vertices.push_back(vertex);
        indices.push_back(i);

        P = P + v * unit;
        v = v - g * unit;
    }

    Mesh* square = new Mesh(name);
    

    square->SetDrawMode(GL_LINE_STRIP);


    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object2D::CreateProjectile(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color), // 0
        VertexFormat(corner + glm::vec3(length, 0, 0), color), // 1
        VertexFormat(corner + glm::vec3(0.86 * length, 0.5 * length, 0), color), // 2
        VertexFormat(corner + glm::vec3(0.5 * length, 0.86 * length, 0), color), // 3
        VertexFormat(corner + glm::vec3(0, length, 0), color), // 4
        VertexFormat(corner + glm::vec3(-0.5 * length, 0.86 * length, 0), color), // 5
        VertexFormat(corner + glm::vec3(-0.86 * length, 0.5 * length, 0), color), // 6
        VertexFormat(corner + glm::vec3(-length, 0, 0), color), // 7
        VertexFormat(corner + glm::vec3(-0.86 * length, -0.5 * length, 0), color), // 8
        VertexFormat(corner + glm::vec3(-0.5 * length, -0.86 * length, 0), color), // 9
        VertexFormat(corner + glm::vec3(0, -length, 0), color), // 10
        VertexFormat(corner + glm::vec3(0.5 * length, -0.86 * length, 0), color), // 11
        VertexFormat(corner + glm::vec3(0.86 * length, -0.5 * length, 0), color), // 12


    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1};

    square->SetDrawMode(GL_TRIANGLE_FAN);


    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object2D::CreateLine(
    const std::string& name, glm::vec3 start, glm::vec3 end, glm::vec3 color)
{


    std::vector<VertexFormat> vertices = {
        VertexFormat(start, color), // 0
        VertexFormat(end, color), // 1

    };
    std::vector<unsigned int> indices = {0, 1};

    Mesh* square = new Mesh(name);

    square->SetDrawMode(GL_LINE_STRIP);
    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color), // 0
        VertexFormat(corner + glm::vec3(length, 0, 0), color), // 1
        VertexFormat(corner + glm::vec3(length * 0.2, length * 0.2, 0), color), // 2
        VertexFormat(corner + glm::vec3(0, length, 0), color), // 3
        VertexFormat(corner + glm::vec3(-length * 0.2, length * 0.2, 0), color), // 4
        VertexFormat(corner + glm::vec3(-length, 0, 0), color), // 5
        VertexFormat(corner + glm::vec3(-length * 0.2, -length * 0.2, 0), color), // 6
        VertexFormat(corner + glm::vec3(0, -length, 0), color), // 7
        VertexFormat(corner + glm::vec3(length * 0.2, -length * 0.2, 0), color), // 8

    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 1 };

    square->SetDrawMode(GL_TRIANGLE_FAN);


    square->InitFromData(vertices, indices);
    return square;
}