#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object3d
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    Mesh* CreatePara(const std::string& name, glm::vec3 center, float height, float width, float length, glm::vec3 color);

    Mesh* CreateTerrain(const std::string& name, int vertCount, glm::vec3 botLeftCorner, float cellWidth, glm::vec3 color);

    Mesh* CreateCylinder(const std::string& name, glm::vec3 center, float height, float radius, glm::vec3 color);

    Mesh* CreateCone(const std::string& name, glm::vec3 center, float height, float radius, glm::vec3 color);

    Mesh* CreateArrow(const std::string& name, glm::vec3 center, float len, glm::vec3 color);

    Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color);


}
