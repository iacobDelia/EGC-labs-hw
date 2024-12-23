#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2DNew.h"
#include "lab_m1/Tema1/object2D.h"
using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

float Tema1::constrFunc(float X) {

   // return 200 + 100 * sin(X);
    //return 200 + 100 * sin( X) + 10 * sin ( X);
   // return 300 + 30 * sin(0.01 * X) + 200 * sin(0.1 * X) + 50 * sin(3 * X);
    return 300 + 40 * sin(0.02 * X)  + 50 * sin(0.01 * X);

}
void Tema1::Init()
{

    srand(time(NULL));
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    float unit = resolution.x / numbPoints;

    float crtPointX = 0, crtPointY;
    float cnt = 0;
    float cntSin = 0;
    srand(time(NULL));
    int random_value = (rand() % 20) * 100;
    // calculate the initial terrain configuration
    for (int i = 0; i < numbPoints; i++) {
        // cnt in 0...1
        crtPointY = constrFunc(crtPointX + random_value);
        glm::vec3 myVec(crtPointX, crtPointY, 0);
        pointList.push_back(myVec);

        crtPointX += unit;

        // create the squares too
        Mesh* square = object2D::CreateSquare("square" + std::to_string(i), glm::vec3(0, 0, 0), 1, glm::vec3(1, 0.5f, 0), true);
        addToList(square);
    }
    tank1PosInd = 30;
    tank2PosInd = 470;

    Mesh* tank1 = object2D::CreateTank("tank1", corner, 70, glm::vec3(1, 0, 0.7), true);
    Mesh* cannon1 = object2D::CreateCannon("cannon1", glm::vec3(0, 0, 0), 100, glm::vec3(1, 0, 0.7), true);

    Mesh* tank2 = object2D::CreateTank("tank2", corner, 70, glm::vec3(1, 0, 0), true);
    Mesh* cannon2 = object2D::CreateCannon("cannon2", glm::vec3(0, 0, 0), 100, glm::vec3(1, 0, 0), true);

    Mesh* projectile = object2D::CreateProjectile("projectile", corner, 5, glm::vec3(1, 1, 1), true);
    Mesh* trajectory = object2D::CreateTrajectory("trajectory", corner, 20, glm::vec3(1, 1, 1), true);

    Mesh* healthBox = object2D::CreateSquare("healthBox", corner, 20, glm::vec3(1, 1, 1), false);
    Mesh* health = object2D::CreateSquare("health", corner, 20, glm::vec3(1, 1, 1), true);
    Mesh* star1 = object2D::CreateStar("starWhite", corner, 15, glm::vec3(1, 1, 1), true);
    Mesh* star2 = object2D::CreateStar("starGray", corner, 15, glm::vec3(0.7, 0.7, 0.7), true);
    Mesh* explosionRed = object2D::CreateStar("explosionRed", glm::vec3(0, 0, 1), 15, glm::vec3(1, 0, 0), true);
    Mesh* explosionYellow = object2D::CreateStar("explosionYellow", glm::vec3(0, 0, 2), 15, glm::vec3(1, 1, 0), true);

    // we need to know the centre to calculate the cannon's origin
    tankCentre = pointList[tank1PosInd] + glm::vec3(0, 20, 1);
    AddMeshToList(tank1);
    AddMeshToList(cannon1);

    AddMeshToList(tank2);
    AddMeshToList(cannon2);

    AddMeshToList(projectile);
    AddMeshToList(trajectory);
    AddMeshToList(healthBox);
    AddMeshToList(health);
    AddMeshToList(star1);
    AddMeshToList(star2);
    AddMeshToList(explosionRed);
    AddMeshToList(explosionYellow);


    
}

void Tema1::addToList(Mesh* mesh) {
    if (mesh->GetMeshID())
    {
        squareList[mesh->GetMeshID()] = mesh;
    }
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    renderTerrain();
    landSlide(deltaTimeSeconds);
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DNew::Translate(900, 600);

    if (tank1Life > 0) {
        renderTrajectory(deltaTimeSeconds, tank1PosInd, trajectory1);
        renderTank(meshes["tank1"], meshes["cannon1"], meshes["trajectory"], int(tank1PosInd), deltaTimeSeconds, trajectory1, tank1Life);

    }
    if (tank2Life > 0) {
        if(!manualControl)
            updateEnemyTank(deltaTimeSeconds);
        renderTrajectory(deltaTimeSeconds, tank2PosInd, trajectory2);
        renderTank(meshes["tank2"], meshes["cannon2"], meshes["trajectory"], int(tank2PosInd), deltaTimeSeconds, trajectory2, tank2Life);
    }
    updateExplosions(deltaTimeSeconds);

    renderStars(deltaTimeSeconds);
    renderProjectiles(deltaTimeSeconds);
}
void Tema1::updateEnemyTank(float deltaTimeSeconds) {
    // 1 second timer
    tank2Timer += deltaTimeSeconds;
    int random;
    if (tank2Timer - 1 > 0) {
        tank2Timer -= 1;
        // choose a random move once every 1 seconds
        tank2Move = (rand() % 10);
        tank2Move += 1;
        hasLaunched = false;
    }
    // move left
    if (1 <= tank2Move && tank2Move <= 3) {
        if (tank2PosInd - tankSpd * deltaTimeSeconds > 0) {
            tank2PosInd -= tankSpd * deltaTimeSeconds;
        }
    }
    // move right
    if (tank2Move == 5 || tank2Move == 6) {
        glm::ivec2 resolution = window->GetResolution();
        if (tank2PosInd + tankSpd * deltaTimeSeconds < pointList.size() - 1 && tank2PosInd + tankSpd * deltaTimeSeconds < resolution.x - 1);
            tank2PosInd += tankSpd * deltaTimeSeconds;
    }
    // move cannon to the left
    if (tank2Move == 7 || tank2Move == 8) {
        if (rotRate2 < 0.2) {
            rotRate2 += cannonSpd * deltaTimeSeconds;
            trajectory2 += 5 * deltaTimeSeconds;
        }
    }
    // move cannon to the rigth
    if (tank2Move == 9) {
        if (-0.2 < rotRate2) {
            rotRate2 -= cannonSpd * deltaTimeSeconds;
            trajectory2 -= 5 * deltaTimeSeconds;
        }
    }
    // launch projectile
    if (tank2Move == 10 && hasLaunched == false) {
        hasLaunched = true;
        projectileList.push_back(new Projectile(tank2PosInd, 2, this, trajectory2));
    }

}

void Tema1::updateExplosions(float deltaTimeSeconds) {
    // update the timer for all explosions
    for (int i = 0; i < expList.size(); i++) {
        expList[i].timer += deltaTimeSeconds;
        if (expList[i].timer < 0.2) {
            expList[i].scale += 0.3;

            modelMatrix = transform2DNew::Translate(pointList[expList[i].ind].x, pointList[expList[i].ind].y);
            modelMatrix *= transform2DNew::Scale(expList[i].scale, expList[i].scale);
            RenderMesh2D(meshes["explosionRed"], shaders["VertexColor"], modelMatrix);
            modelMatrix *= transform2DNew::Rotate(2.5);
            RenderMesh2D(meshes["explosionRed"], shaders["VertexColor"], modelMatrix);
            modelMatrix *= transform2DNew::Scale(0.5, 0.5);
            RenderMesh2D(meshes["explosionYellow"], shaders["VertexColor"], modelMatrix);
            modelMatrix *= transform2DNew::Rotate(-2.5);
            RenderMesh2D(meshes["explosionYellow"], shaders["VertexColor"], modelMatrix);

        }
        if (expList[i].timer > 0.2 && expList[i].hasExploded == false) {
            expList[i].hasExploded = true;
            explodeTerrain(expList[i].ind);
        }
    }

}

void Tema1::renderStars(float deltaTimeSeconds) {
    if (starAnimCondition == true)
        starAnimTimer += 1;
    else starAnimTimer -= 1;

    if (starAnimTimer == 30)
        starAnimCondition = false;
    if (starAnimTimer == 0)
        starAnimCondition = true;
    renderOneStar(starAnimCondition, glm::vec3(730, 600, 0));
    renderOneStar(starAnimCondition, glm::vec3(300, 670, 0));
    renderOneStar(starAnimCondition, glm::vec3(1000, 600, 0));
    renderOneStar(starAnimCondition, glm::vec3(1200, 500, 0));
    renderOneStar(starAnimCondition, glm::vec3(100, 550, 0));


}
void Tema1::renderOneStar(bool state, glm::vec3 pos) {
    glm::mat3 modelMatrix = transform2DNew::Translate(pos.x, pos.y);
    if (state == false) {
        RenderMesh2D(meshes["starWhite"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2DNew::Rotate(2.5);
        RenderMesh2D(meshes["starGray"], shaders["VertexColor"], modelMatrix);

    }
    else {
        modelMatrix *= transform2DNew::Rotate(2.5);
        RenderMesh2D(meshes["starWhite"], shaders["VertexColor"], modelMatrix);
        modelMatrix = transform2DNew::Translate(pos.x, pos.y);
        RenderMesh2D(meshes["starGray"], shaders["VertexColor"], modelMatrix);
    }
}
void Tema1::renderTrajectory(float deltaTimeSeconds, int tankPosInd, float trajectory) {
    
    // this is the same formula as used in the constructor for a projectile
    modelMatrix = glm::mat3(1);
    // find where exactly to spawn the projectile
    glm::vec3 V = pointList[tankPosInd + 1] - pointList[tankPosInd];
    glm::vec3 mid = glm::vec3(abs(pointList[tankPosInd + 1].x + pointList[tankPosInd].x) / 2,
        abs(pointList[tankPosInd + 1].y + pointList[tankPosInd].y) / 2, 0);
    if (pointList[tankPosInd].y < 0 || pointList[tankPosInd + 1].y < 0) {
        mid.y = 0;
    }

    float alph = atan2(V.y, V.x);
    glm::mat3 turPos = transform2DNew::Translate(0, 10.0);
    glm::mat3 tankTrans = transform2DNew::Translate(mid.x, mid.y) * transform2DNew::Rotate(alph);
    glm::mat3 turretTrans = tankTrans * turPos;

    // apply the transformations to the coordinates
    glm::vec3 turPlaceLocal = glm::vec3(0, 20, 1);
    glm::vec3 turPlaceGlobal = turretTrans * turPlaceLocal;

    glm::vec2 P = glm::vec2(turPlaceGlobal.x, turPlaceGlobal.y);
    float direction = 1;

    glm::vec2 v = glm::vec2(cos(trajectory), sin(trajectory)) * glm::vec2(300, 300);
    glm::vec2 g = glm::vec2(0, 300);


    float lineSize = 0.02;
    glm::mat3 modelMatrix = glm::mat3(1);

    glm::vec3 crtPos = turPlaceGlobal;
    glm::vec3 prevPos;
    int cnt = 0;
    while (crtPos.y > 0) {
        prevPos = crtPos;
        P = P + v * glm::vec2(lineSize, lineSize);
        v = v - g * glm::vec2(lineSize, lineSize);
        crtPos = glm::vec3(P, 0);
        // create lines at the exact coordinates we need
        // we dont put them in a list so it should be fine memory wise?
        Mesh* line = object2D::CreateLine("line", prevPos, crtPos, glm::vec3(1, 1, 1));
        if(cnt > 1)
            RenderMesh2D(line, shaders["VertexColor"], modelMatrix);
        cnt++;

    }

}

void Tema1::renderProjectiles(float deltaTimeSeconds) {
    for (Projectile* projectile : projectileList) {
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], projectile->modelMatrix);
        projectile->updateProjectile(deltaTimeSeconds, this);
    }
}
void Tema1::renderTank(Mesh* tankMesh, Mesh* cannonMesh, Mesh* trajectoryMesh, int tankPosInd, float deltaTimeSeconds, float rotRate,
                        int health){
    // calculate tank position and orientation
    glm::vec3 V = pointList[tankPosInd + 1] - pointList[tankPosInd];
    float alph = atan2(V.y, V.x);
    // calculate middle
    
    glm::vec3 mid = glm::vec3(abs(pointList[tankPosInd + 1].x + pointList[tankPosInd].x) / 2,
        abs(pointList[tankPosInd + 1].y + pointList[tankPosInd].y) / 2, 0);
    if (pointList[tankPosInd].y < 0 || pointList[tankPosInd + 1].y < 0) {
        mid.y = 0;
        alph = 0;
    }
        

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DNew::Translate(mid.x, mid.y);
    modelMatrix *= transform2DNew::Rotate(alph);
    RenderMesh2D(tankMesh, shaders["VertexColor"], modelMatrix);

    // render the health box boundary
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DNew::Translate(mid.x - 30, mid.y + 100);
    modelMatrix *= transform2DNew::Scale(3, 1);
    RenderMesh2D(meshes["healthBox"], shaders["VertexColor"], modelMatrix);

    // render the health
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DNew::Translate(mid.x - 30, mid.y + 100);
    modelMatrix *= transform2DNew::Scale(3, 1);
    float percent = 0;
    for (int i = 0; i < health; i++) {
        percent += 0.2;
    }
    modelMatrix *= transform2DNew::Scale(percent, 1);
    RenderMesh2D(meshes["health"], shaders["VertexColor"], modelMatrix);

    // render the cannon
    modelMatrix = glm::mat3(1);

    // we want to put it a little off the ground
    glm::mat3 turPos = transform2DNew::Translate(0, 20.0f);
    glm::mat3 tankTrans = transform2DNew::Translate(mid.x, mid.y) * transform2DNew::Rotate(alph);
    // calculate it like this to find the rotation better
    glm::mat3 turretTrans = tankTrans * turPos * transform2DNew::Rotate(rotRate - alph - 1.5);

    RenderMesh2D(cannonMesh, shaders["VertexColor"], turretTrans);

}

void Tema1::landSlide(float deltaTimeSeconds) {
    float d = 5;

    // update for each explosion
    for (int j = 0; j < expList.size(); j++) {
        if (expList[j].timer < 3) {
            int start = expList[j].ind - Rint - 20;
            int end = expList[j].ind + Rint + 20;
            // check if were in bounds
            if (start < 0)
                start = 0;
            if (end > pointList.size())
                end = pointList.size() - 1;
            for (int i = start; i < end; i++) {
                // update with the difference if the points are too far away
                float pDistance = abs(pointList[i].y - pointList[i + 1].y);
                pDistance *= deltaTimeSeconds;
                pDistance *= 2;
                if (pointList[i].y - pointList[i + 1].y > d) {
                    pointList[i].y -= pDistance;
                    pointList[i + 1].y += pDistance;
                }
                if (pointList[i + 1].y - pointList[i].y > d) {
                    pointList[i + 1].y -= pDistance;
                    pointList[i].y += pDistance;
                }
            }
        }
        
    }

}
void Tema1::explodeTerrain(int center) {
    int start = center - Rint;
    int end = center + Rint;
    if (start < 0)
        start = 0;
    glm::vec3 centerCoord = pointList[center];
    if (start > 0 && end < numbPoints) {
        // calculate actual radius
        float realR = centerCoord.x - pointList[start].x;
        float lowestPoint = centerCoord.y - realR;
        for (int i = start; i < end; i++) {
            float difference = realR; 
            if (pointList[i].y >= lowestPoint) {
                if (i != center) {
                    float distFromCenter = abs(pointList[i].x - centerCoord.x);
                    difference = sqrt(realR * realR - distFromCenter * distFromCenter);

                    if(centerCoord.y > pointList[i].y)
                        difference = difference - (centerCoord.y - pointList[i].y);
                    else
                        difference = difference +  (pointList[i].y - centerCoord.y);
                }
                if(difference > 0)
                    pointList[i].y = pointList[i].y - difference;
            }
        }
    }
    
}
void Tema1::renderTerrain() {
    glm::mat3 modelMatrix;
    glm::ivec2 resolution = window->GetResolution();

    float cnt = 0;

    for (int i = 0; i < numbPoints - 1; i++) {
        modelMatrix = glm::mat3(1);
        glm::vec2 crtPoint = pointList[i];
        glm::vec2 nextPoint = pointList[i + 1];
        modelMatrix *= transform2DNew::Translate(crtPoint.x, crtPoint.y);

        float shear = (nextPoint.y - crtPoint.y) / (nextPoint.x - crtPoint.x);

        modelMatrix *= transform2DNew::Shear(shear, 0);

        modelMatrix *= transform2DNew::Scale(nextPoint.x - crtPoint.x, max(crtPoint.y, nextPoint.y));
        RenderMesh2D(squareList["square" + std::to_string(i)], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    glm::ivec2 resolution = window->GetResolution();

    if (window->KeyHold(GLFW_KEY_D)) {
        if (tank1PosInd + tankSpd * deltaTime < resolution.x - 1 && tank1PosInd + tankSpd * deltaTime < pointList.size() - 1)
            tank1PosInd += tankSpd * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        if(tank1PosInd - tankSpd * deltaTime > 0)
            tank1PosInd -= tankSpd * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        if (rotRate1 < 0.2) {
            trajectory1 += 5 * deltaTime;
            rotRate1 += cannonSpd * deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        if (-0.2 < rotRate1) {
            rotRate1 -= cannonSpd * deltaTime;
            trajectory1 -= 5 * deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        if (rotRate2 < 0.2) {
            rotRate2 += cannonSpd * deltaTime;
            trajectory2 += 5 * deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        if (-0.2 < rotRate2) {
            rotRate2 -= cannonSpd * deltaTime;
            trajectory2 -= 5 * deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_LEFT)) {
        tank2PosInd -= tankSpd * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        tank2PosInd += tankSpd * deltaTime;
    }

}
// returns the nearest bigger index for an X coordinate
int Tema1::nearestIndex(float X) {
    for (int i = 0; i < pointList.size(); i++) {
        if (X <= pointList[i].x)
            return i;
    }
}

void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) {
        projectileList.push_back(new Projectile(tank1PosInd, 1, this, trajectory1));
    }
    if (key == GLFW_KEY_ENTER) {
        projectileList.push_back(new Projectile(tank2PosInd, 2, this, trajectory2));
    }
    if (key == GLFW_KEY_Q) {
        if (manualControl == false)
            manualControl = true;
        else manualControl = false;
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
