#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/object3d.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraHWTarget = false;

    camera = new implemented::CameraHW();
    cameraOrtho = new implemented::CameraHW();
    dumbCamera = new implemented::CameraHW();

    glm::ivec2 resolution = window->GetResolution();

    Mesh* mesh = object3d::CreatePara("boxGray", glm::vec3(0, 0, 0), 0.15, 0.15, 2.5, glm::vec3(0.8, 0.8, 0.8));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("boxGreen", glm::vec3(0, 0, 0), 0.15, 0.15, 2.5, glm::vec3(0.25, 0.92, 0.18));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("groundOrange", glm::vec3(0, 0, 0), 1, 1, 1, glm::vec3(0.89, 0.5, 0.21));
    meshes[mesh->GetMeshID()] = mesh;
    
    mesh = object3d::CreateArrow("arrow", glm::vec3(0, 0, 0), 1, glm::vec3(1, 0, 0));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreateArrow("arrowDrone", glm::vec3(0, 0, 0), 1, glm::vec3(0, 0, 0));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("cubeGray", glm::vec3(0, 0, 0), 0.15, 0.15, 0.15, glm::vec3(0.8, 0.8, 0.8));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("building", glm::vec3(0, 0, 0), 1, 1, 1, glm::vec3(0.45, 0.43, 0.4));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("cubeGreen", glm::vec3(0, 0, 0), 0.15, 0.15, 0.15, glm::vec3(0.25, 0.92, 0.18));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("propeller", glm::vec3(0, 0, 0), 0.05, 0.05, 0.7, glm::vec3(0.1, 0.1, 0.1));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("bigTerrain", glm::vec3(0, 0, 0), 1, 1, 1, glm::vec3(0.3, 0.6, 0.6));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreateTerrain("terrain", 100, glm::vec3(0, 0, 0), 1, glm::vec3(0.52, 0.16, 0.09));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreateCylinder("cylinder", glm::vec3(0, 0, 0), 1, 1, glm::vec3(0.5, 0.2, 0.11));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreateCone("cone", glm::vec3(0, 0, 0), 1, 1, glm::vec3(0.84, 0.46, 0.2));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("packet", glm::vec3(0, 0, 0), 1, 1, 1, glm::vec3(0.97, 0.89, 0.6));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("enemyBlock", glm::vec3(0, 0, 0), 1, 1, 1, glm::vec3(0.4, 0.7, 0.8));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("healthBlock", glm::vec3(0, 0, 0), 1, 1, 1, glm::vec3(0.25, 0.92, 0.18));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreatePara("enemyBlockActive", glm::vec3(0, 0, 0), 1, 1, 1, glm::vec3(0.7, 0.9, 1));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreateCylinder("goal", glm::vec3(0, 0, 0), 0.2, goalRadius, glm::vec3(1, 0.2, 0.1));
    meshes[mesh->GetMeshID()] = mesh;
    mesh = object3d::CreateCircle("goalCircle", glm::vec3(0, 0, 0), goalRadius, glm::vec3(1, 0.2, 0.1));
    meshes[mesh->GetMeshID()] = mesh;

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);


    srand(time(NULL));
    // randomly place trees
    int numTrees = (rand() % 20) + 10;
    for (int i = 0; i < numTrees; i++) {

        glm::vec3 randPlace = findRandomPlace();
        float height = (rand() % 10) + 4;
        int numSeg = (rand() % 2) + 2;

        float dx = (dronePos.x - randPlace.x) * (dronePos.x - randPlace.x);
        float dz = (dronePos.y - randPlace.z) * (dronePos.z - randPlace.z);
        float dist = sqrt(dx + dz);
        if(dist > 5)
            treeList.push_back(Tree(glm::vec3(randPlace.x, 0, randPlace.z), numSeg, height));
    }
    int numBuildings = (rand() % 5) + 3;
    for (int i = 0; i < numBuildings; i++) {
        glm::vec3 randPlace = findRandomPlace();

        float dx = (dronePos.x - randPlace.x) * (dronePos.x - randPlace.x);
        float dz = (dronePos.y - randPlace.z) * (dronePos.z - randPlace.z);
        float dist = sqrt(dx + dz);
        if (dist > 10) {
            float x = (rand() % 10) + 1;
            float y = (rand() % 10) + 1;
            float z = (rand() % 10) + 1;
            buildings.push_back(Building(randPlace, glm::vec3(x, y, z)));
        }
    }
    packetPos = findRandomPlace();
    goalPos = findRandomPlace();
    // add our shaders
    Shader* shader = new Shader("grassShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "grassVertex.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "grassFragment.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    shader = new Shader("treeShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "treeVertex.glsl"), GL_VERTEX_SHADER);

    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "treeFrag.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
    miniArea = glm::vec2(resolution.x / 4, resolution.y / 4);
    cameraOrthoPos = dronePos + glm::vec3(0, 30, 0);
    cameraOrtho->position = glm::vec3(0, 0, 0);
    cameraOrtho->RotateFirstPerson_OX(RADIANS(90));
    cameraOrtho->RotateFirstPerson_OY(RADIANS(180));
    dumbCamera->position = glm::vec3(0, -5, 0);
    dumbCamera->RotateFirstPerson_OX(RADIANS(90));
    dumbCamera->RotateFirstPerson_OY(RADIANS(180));
    
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

}


void Tema2::Update(float deltaTimeSeconds)
{
    updateTimers(deltaTimeSeconds);
    glm::ivec2 resolution = window->GetResolution();
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, -3, 0));
    
    modelMatrix = glm::scale(modelMatrix, glm::vec3(10000, 1, 10000));
    // my drone
    glViewport(0, 0, resolution.x, resolution.y);

    renderDrone(meshes["boxGray"], meshes["cubeGray"], droneRotOy, propRot, deltaTimeSeconds, dronePos, camera);
    glm::vec3 tempPos = dronePos;

    updatePackets(deltaTimeSeconds);

    updateFriend(deltaTimeSeconds, &crtFriend);
    updatePropRot(deltaTimeSeconds);
    renderTerrain(deltaTimeSeconds, glm::vec3(-30, 0, -30), "grassShader", camera);
    renderTrees(deltaTimeSeconds, camera);
    renderBuildings(camera);
    if (isPacketGameOn) {
        updatePacketGame(deltaTimeSeconds, camera);
    }
    RenderMesh(meshes["groundOrange"], shaders["VertexColor"], modelMatrix, camera);
    if (areEnemiesOn) {
        renderEnemies(deltaTimeSeconds, camera);
    }


    glClear(GL_DEPTH_BUFFER_BIT);

    // draw the border
    glViewport(10, resolution.y - miniArea.y - 30, miniArea.x + 20, miniArea.y + 20);
    RenderMesh(meshes["cubeGray"], shaders["VertexColor"], modelMatrix, dumbCamera);
    glClear(GL_DEPTH_BUFFER_BIT);

    // draw the ground
    glViewport(20, resolution.y - miniArea.y - 20, miniArea.x, miniArea.y);
    RenderMesh(meshes["groundOrange"], shaders["VertexColor"], modelMatrix, dumbCamera);
    glClear(GL_DEPTH_BUFFER_BIT);

    // draw the ortho projection
    glm::mat4 prevProj = projectionMatrix;
    projectionMatrix = glm::ortho(left, right, bottom, top, 0.01f, 20000.0f);

    glViewport(20, resolution.y - miniArea.y - 20, miniArea.x, miniArea.y);


    RenderMesh(meshes["groundOrange"], shaders["VertexColor"], modelMatrix, cameraOrtho);
    if (isPacketPicked) {
        renderArrow(deltaTimeSeconds, dronePos, goalPos, cameraOrtho);
    }
    else {
        renderArrow(deltaTimeSeconds, dronePos, packetPos, cameraOrtho);
    }
    renderPacketGame(deltaTimeSeconds, cameraOrtho);
    renderDroneArrow(deltaTimeSeconds, dronePos, cameraOrtho);
    renderTrees(deltaTimeSeconds, cameraOrtho);
    renderBuildings(cameraOrtho);
    projectionMatrix = prevProj;
}

void Tema2::updatePackets(float deltaTimeSeconds) {
    for (auto& crtPacket : healthPackets) {
        
        Tema2::Packet* packet = &crtPacket;
        if (!packet->isPicked) {
            if (packet->isFalling) {
                packet->pos.y -= deltaTimeSeconds * packFallSpd;
                if (packet->pos.y <= 0.3) {
                    packet->isFalling = false;
                }
            }
            if (isInsidePar(packet->pos, glm::vec3(1, 1, 1), dronePos)) {
                health += 1;
                cout << "Health restored by 1! Current health: " << health << "\n";
                packet->isPicked = true;
            }
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1), packet->pos);
            RenderMesh(meshes["healthBlock"], shaders["VertexColor"], modelMatrix, camera);
        }

    }
}
void Tema2::updateTimers(float deltaTimeSeconds) {
    spawnCounterEnemy += deltaTimeSeconds;
    if (spawnCounterEnemy >= spawnTimeEnemy) {
        spawnCounterEnemy = 0;
        // add an enemy every 5 seconds
        glm::vec3 randPlace = findRandomPlace();
        enemies.push_back(Enemy(randPlace));
    }
    friendTimer += deltaTimeSeconds;
    if (friendTimer >= friendInterval) {
        if (!isFriendSpawned) {
            glm::vec3 randPlace = findRandomPlace();
            randPlace.y = 7;
            randPlace.x = -22;
            
            cout << "spawned friend\n";
            crtFriend = Tema2::FriendDr(randPlace);
            isFriendSpawned = true;
        }
    }
}
void Tema2::updateFriend(float deltaTimeSeconds, Tema2::FriendDr* crtFriend) {
    if (isFriendSpawned) {
        
        renderDrone(meshes["boxGreen"], meshes["cubeGreen"], 0, propRot, deltaTimeSeconds, crtFriend->pos, camera);
        crtFriend->pos.x += deltaTimeSeconds * friendSpeed;
        crtFriend->packetPos.x += deltaTimeSeconds * friendSpeed;
        if (crtFriend->hasPacket) {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1), crtFriend->packetPos);
            RenderMesh(meshes["healthBlock"], shaders["VertexColor"], modelMatrix, camera);
        }
        if (crtFriend->pos.x >= crtFriend->randDistance) {
            crtFriend->hasPacket = false;
            if (!crtFriend->hasAddedPckToArr) {
                healthPackets.push_back(Packet(crtFriend->packetPos));
                crtFriend->hasAddedPckToArr = true;
            }
        }
        if (crtFriend->pos.x >= 77) {
            isFriendSpawned = false;
            friendTimer = 0;
        }
    }
}
void Tema2::renderExPoint(glm::vec3 pos) {
    glm::vec3 scaleDot = glm::vec3(0.5, 0.5, 0.5);
    glm::vec3 scaleLine = glm::vec3(0.5, 3, 0.5);
    
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), pos + glm::vec3(0, 4, 0));
    modelMatrix = glm::scale(modelMatrix, scaleDot);
    RenderMesh(meshes["enemyBlock"], shaders["VertexColor"], modelMatrix, camera);

    modelMatrix = glm::translate(glm::mat4(1), pos + glm::vec3(0, 5, 0));
    modelMatrix = glm::scale(modelMatrix, scaleLine);
    RenderMesh(meshes["enemyBlock"], shaders["VertexColor"], modelMatrix, camera);
}
void Tema2::renderEnemy(float deltaTimeSeconds, Tema2::Enemy enemy, implemented::CameraHW* camera) {

    glm::mat4 mat = glm::translate(glm::mat4(1), enemy.pos);
    mat = glm::scale(mat, enemy.dimensions);
    if (enemy.isBlinking) {
        RenderMesh(meshes["enemyBlockActive"], shaders["VertexColor"], mat * enemy.rotMatrix, camera);
    }
    else {
        RenderMesh(meshes["enemyBlock"], shaders["VertexColor"], mat * enemy.rotMatrix, camera);
    }
    mat = glm::translate(glm::mat4(1), enemy.pos + glm::vec3(0, 1.5, 0));
    
    mat = glm::scale(mat, enemy.dimensions - glm::vec3(0.5, 0.5, 0.5));
    if (enemy.isBlinking) {
        RenderMesh(meshes["enemyBlockActive"], shaders["VertexColor"], mat * enemy.rotMatrix, camera);
    }
    else {
        RenderMesh(meshes["enemyBlock"], shaders["VertexColor"], mat * enemy.rotMatrix, camera);
    }
    if (enemy.isExploding) {
        glm::mat4 mat = glm::translate(glm::mat4(1), enemy.pos);
        mat = glm::scale(mat, glm::vec3(enemy.explosionRadius, enemy.explosionRadius, enemy.explosionRadius));
        RenderMesh(meshes["enemyBlockActive"], shaders["VertexColor"], mat * enemy.rotMatrix, camera);
    }
}
void Tema2::updateEnemyIdle(float deltaTimeSeconds, Tema2::Enemy* enemy) {
    enemy->isBlinking = false;
    if (enemy->internalTimer >= enemyTimerMax) {
        enemy->internalTimer = 0;
        int numb = rand() % 8;
        if (numb == 0) {
            enemy->crtAction = IDLE;
        }
        if (numb >= 1 && numb <= 2) {
            enemy->crtAction = ROTLEFT;
        }
        if (numb >= 3 && numb <= 4) {
            enemy->crtAction = ROTRIGHT;
        }
        if (numb >= 5 && numb <= 7) {
            enemy->crtAction = WALK;
        }
    }
    if (enemy->crtAction == IDLE) {

    }
    if (enemy->crtAction == ROTLEFT) {
        float angle = deltaTimeSeconds * enemyRotateSpd;
        enemy->forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(enemy->forward, 1)));
        enemy->rotMatrix = glm::rotate(enemy->rotMatrix, angle, glm::vec3(0, 1, 0));
    }
    if (enemy->crtAction == ROTRIGHT) {
        float angle = deltaTimeSeconds * enemyRotateSpd * (-1);
        enemy->forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(enemy->forward, 1)));
        enemy->rotMatrix = glm::rotate(enemy->rotMatrix, angle, glm::vec3(0, 1, 0));
    }
    if (enemy->crtAction == WALK) {
        glm::vec3 dir = glm::normalize(glm::vec3(enemy->forward.x, 0, enemy->forward.z));
        enemy->pos += dir * deltaTimeSeconds * enemySpd;
    }
}
void Tema2::updateEnemyAgro(float deltaTimeSeconds, Tema2::Enemy* enemy) {
    if (enemy->atkTimer <= 2) {
        renderExPoint(enemy->pos);
    }
    if (enemy->atkTimer > 2 && enemy ->atkTimer < 10) {
        if (enemy->atkTimer >= enemy->blinkInterval + 2) {
            enemy->atkTimer = 2.01;
            enemy->isBlinking = !enemy->isBlinking;
            enemy->blinkInterval /= 1.2;
        }
        if (enemy->blinkInterval <= 0.1) {
            enemy->atkTimer = 10.1;
            enemy->isExploding = true;
        }
    }
    // here were exploding!
    if (enemy->atkTimer >= 10) {
        enemy->explosionRadius += deltaTimeSeconds * 20;
    }
    
    // look at the drone
    glm::mat4 modelMatrix;
    glm::vec3 direction = enemy->pos - dronePos;
    float angle = atan2(direction.x, direction.z);
    enemy->forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1)));
    enemy->rotMatrix = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0));

    if (enemy->explosionRadius >= 14 && !enemy->isDead) {
        enemy->isDead = true;
        health--;
        if (health > 0)
            cout << "You've been hit! Health left: " << health << "\n";
        else {
            cout << "Game over! Try again!" << "\n";
            resetGame();
        }
    }
}
void Tema2::resetGame() {
    dronePos = initPos;
    health = 3;
    enemies.clear();
    healthPackets.clear();
    isPacketPicked = false;
    camera = new implemented::CameraHW();
    cameraOrtho = new implemented::CameraHW();
    cameraOrthoPos = dronePos + glm::vec3(0, 30, 0);
    cameraOrtho->position = glm::vec3(0, 0, 0);
    cameraOrtho->RotateFirstPerson_OX(RADIANS(90));
    cameraOrtho->RotateFirstPerson_OY(RADIANS(180));

    goalPos = findRandomPlace();
    packetPos = findRandomPlace();
    packetsArrived = 0;
}
void Tema2::updateEnemy(float deltaTimeSeconds, Tema2::Enemy* enemy) {
    // check if collision occurred
    if (distance(dronePos, enemy->pos) <= aggroDist) {
        enemy->internalTimer = 0;
        enemy->isAgro = true;
        enemy->atkTimer += deltaTimeSeconds;
        updateEnemyAgro(deltaTimeSeconds, enemy);

    }
    else {
        enemy->atkTimer = 0;
        enemy->isAgro = false;
        enemy->internalTimer += deltaTimeSeconds;
        enemy->blinkInterval = 0.8;
        enemy->explosionRadius = 0;
        // update the timer
        updateEnemyIdle(deltaTimeSeconds, enemy);
    }
    
}
void Tema2::renderEnemies(float deltaTimeSeconds, implemented::CameraHW* camera) {
    for (auto& enemy : enemies) {
        if (!enemy.isDead) {
            updateEnemy(deltaTimeSeconds, &enemy);
            renderEnemy(deltaTimeSeconds, enemy, camera);
        }
    }
}
void Tema2::renderArrow(float deltaTimeSeconds, glm::vec3 dronePos, glm::vec3 packetPos, implemented::CameraHW* camera) {
    glm::mat4 modelMatrix;
    glm::vec3 direction = packetPos - dronePos;
    float angle = atan2(direction.x, direction.z);
    modelMatrix = glm::translate(glm::mat4(1), dronePos + glm::vec3(0, 4, 0));

    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 3));
    RenderMesh(meshes["arrow"], shaders["VertexColor"], modelMatrix, camera);

}
void Tema2::updatePacketGame(float deltaTimeSeconds, implemented::CameraHW* camera) {
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), packetPos);
    if (isPacketPicked) {
        modelMatrix = glm::rotate(modelMatrix, RADIANS(droneRotOy), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::scale(modelMatrix, packetDim);

    RenderMesh(meshes["packet"], shaders["VertexColor"], modelMatrix, camera);
    if (isInsidePar(packetPos, packetDim, dronePos)) {
        isPacketPicked = true;
    }
    modelMatrix = glm::translate(glm::mat4(1), goalPos);
    RenderMesh(meshes["goal"], shaders["VertexColor"], modelMatrix, camera);

    if (isInsideCylinder(goalPos, goalRadius, 0.5, dronePos) && isPacketPicked) {
        packetsArrived += 1;
        isPacketPicked = false;
        goalPos = findRandomPlace();
        packetPos = findRandomPlace();
        cout << "packets picked: " << packetsArrived << endl;
    }
}
void Tema2::renderPacketGame(float deltaTimeSeconds, implemented::CameraHW* camera) {
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), packetPos + glm::vec3(0, 3, 0));
    if (isPacketPicked) {
        modelMatrix = glm::rotate(modelMatrix, RADIANS(droneRotOy), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::scale(modelMatrix, packetDim);

    RenderMesh(meshes["packet"], shaders["VertexColor"], modelMatrix, camera);
    modelMatrix = glm::translate(glm::mat4(1), goalPos + glm::vec3(0, 3, 0));
    RenderMesh(meshes["goalCircle"], shaders["VertexColor"], modelMatrix, camera);
}

glm::vec3 Tema2::findRandomPlace() {
    float x = (rand() % 100) - 30;
    float z = (rand() % 100) - 30;
    return glm::vec3(x, 0, z);
}
void Tema2::updatePacket() {
    if (isPacketPicked) {
        packetPos = dronePos;
        packetPos.y -= packetDim.y + 0.005;
    }
}

void Tema2::renderTerrain(float deltaTimeSeconds, glm::vec3 position, std::string shader, implemented::CameraHW* camera) {
    glm::mat4 pos = glm::translate(glm::mat4(1), position);
    RenderMesh(meshes["terrain"], shaders[shader], pos, camera);

    pos = glm::translate(glm::mat4(1), glm::vec3(0, -1, 0));
    glm::mat4 bigScale = glm::scale(pos, glm::vec3(1000, 1, 1000));
}

void Tema2::renderBuildings(implemented::CameraHW* camera) {
    for (auto& building : buildings) {
        glm::mat4 modelMatrix;
        modelMatrix = glm::translate(glm::mat4(1), building.pos);
        modelMatrix = glm::scale(modelMatrix, building.dim);
        if (camera == cameraOrtho) {
            glm::vec3 pos = building.pos;
            pos.y = 4;
            modelMatrix = glm::translate(glm::mat4(1), pos);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(building.dim.x, 1, building.dim.z));
        }
        RenderMesh(meshes["building"], shaders["VertexColor"], modelMatrix, camera);
    }
}

void Tema2::renderTrees(float deltaTimeSeconds, implemented::CameraHW* camera) {
    for (auto& tree : treeList) {
        renderTree(deltaTimeSeconds, tree, camera);
    }
}
bool Tema2::isInsideCylinder(glm::vec3 cylPos, float radius, float height, glm::vec3 drPos) {
    // if it is at the same level as the cylinder
    if (drPos.y >= cylPos.y && drPos.y <= cylPos.y + height) {
        // calculate distance
        float xd = (cylPos.x - drPos.x) * (cylPos.x - drPos.x);
        float zd = (cylPos.z - drPos.z) * (cylPos.z - drPos.z);
        float dist = sqrt(xd + zd);
        if (dist <= radius + 1) {
            return true;
        }
        return false;
    }
    return false;
}
bool Tema2::isInsideCone(glm::vec3 cylPos, float radius, float height, glm::vec3 drPos) {
    // if it is at the same level as the cone
    if (drPos.y >= cylPos.y && drPos.y <= cylPos.y + height) {
        // calculate distance
        float xd = (cylPos.x - drPos.x) * (cylPos.x - drPos.x);
        float zd = (cylPos.z - drPos.z) * (cylPos.z - drPos.z);
        float dist = sqrt(xd + zd);
        
        float crtConeRadius = (radius * (height - drPos.y)) / height;

        if (dist <= crtConeRadius + 3) {
            return true;
        }
        return false;
    }
    return false;
}



bool Tema2::isCollided(glm::vec3 drPos) {
    for (auto& tree : treeList) {
        if (isInsideCylinder(tree.pos, treeRadius, tree.trunkHeight, drPos))
            return true;
        for (auto& cone : tree.cones) {
            if (isInsideCone(cone.pos, cone.radius, cone.height, drPos))
                return true;
        }
    }
    for (auto& building : buildings) {
        if (isInsidePar(building.pos, building.dim + glm::vec3(0.1, 0.1, 0.1), drPos)) {
            return true;
        }
    }
    return false;
}
void Tema2::renderTree(float deltaTimeSeconds, Tema2::Tree tree, implemented::CameraHW* camera) {
    glm::mat4 posMat = glm::translate(glm::mat4(1), tree.pos);
    posMat = glm::scale(posMat, glm::vec3(treeRadius, tree.trunkHeight, treeRadius));
    RenderMesh(meshes["cylinder"], shaders["VertexColor"], posMat, camera);

    for (auto& cone : tree.cones) {
        posMat = glm::translate(glm::mat4(1), cone.pos);
        posMat = glm::scale(posMat, glm::vec3(cone.radius, cone.height, cone.radius));

        RenderCone(meshes["cone"], shaders["treeShader"], posMat, cone.pos, cone.radius, camera);
    }

}
void Tema2::renderDroneArrow(float deltaTimeSeconds, glm::vec3 position, implemented::CameraHW* camera) {
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), dronePos + glm::vec3(0, 3, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(droneRotOy) + RADIANS(180), glm::vec3(0, 1, 0));
    RenderMesh(meshes["arrowDrone"], shaders["VertexColor"], modelMatrix, camera);

}
void Tema2::renderDrone(Mesh* block, Mesh* cube, float droneRotOy, float propRot, float deltaTimeSeconds, glm::vec3 position, implemented::CameraHW* camera) {
    
    glm::mat4 posMatrix = glm::mat4(1), modelMatrix = glm::mat4(1);
    glm::mat4 rotMatrix = glm::mat4(1);

    posMatrix = glm::translate(modelMatrix, position);
    rotMatrix = glm::rotate(rotMatrix, RADIANS(droneRotOy), glm::vec3(0, 1, 0));

    posMatrix *= rotMatrix;
    // render the cross
    modelMatrix = glm::rotate(posMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
    RenderMesh(block, shaders["VertexColor"], modelMatrix, camera);

    modelMatrix = glm::rotate(posMatrix, RADIANS(90.f), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
    RenderMesh(block, shaders["VertexColor"], modelMatrix, camera);

    // render the cubes
    int cnt = 45;
    for (int i = 0; i < 4; i++) {
        modelMatrix = glm::rotate(posMatrix, RADIANS(90.f), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(cnt), glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.15, 1.175));

        RenderMesh(cube, shaders["VertexColor"], modelMatrix, camera);
        cnt += 90;
    }
    // render the propellers
    cnt = 45;
    for (int i = 0; i < 4; i++) {
        modelMatrix = glm::rotate(posMatrix, RADIANS(90.f), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(cnt), glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.32, 1.175));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(propRot), glm::vec3(0, 1, 0));

        RenderMesh(meshes["propeller"], shaders["VertexColor"], modelMatrix, camera);
        cnt += 90;
    }

}
void Tema2::updatePropRot(float deltaTime) {
    if (propRot == 360)
        propRot = 0;
    propRot+= deltaTime * 500;
}
void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

bool Tema2::isInsidePar(glm::vec3 cen, glm::vec3 dim, glm::vec3 drPos) {
    // if its inside height
    if (cen.y <= drPos.y && cen.y + dim.y >= drPos.y) {
        float w1 = cen.x - dim.x / 2;
        float w2 = cen.x + dim.x / 2;
        if (w1 <= drPos.x && w2 >= drPos.x) {
            float l1 = cen.z - dim.z / 2;
            float l2 = cen.z + dim.z / 2;
            if (l1 <= drPos.z && l2 >= drPos.z)
                return true;
        }
    }
    return false;
}

void Tema2::RenderCone(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 center, float radius, implemented::CameraHW* camera)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();


    int location_radius = glGetUniformLocation(shaders["treeShader"]->program, "Radius");
    glUniform1f(location_radius, radius);

    int location_time = glGetUniformLocation(shaders["treeShader"]->program, "Time");
    glUniform1f(location_time, Engine::GetElapsedTime());

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, implemented::CameraHW* camera)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 10;

        if (window->KeyHold(GLFW_KEY_W)) {
            glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));

            if (!isCollided(dronePos + dir * cameraSpeed * deltaTime)) {
                dronePos = camera->MoveForward(cameraSpeed * deltaTime, true);
                updatePacket();
                cameraOrtho->MoveForward(cameraSpeed * deltaTime, false);
            }
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            glm::vec3 dir = glm::normalize(glm::vec3(camera->right.x, 0, camera->right.z));

            if (!isCollided(dronePos + dir * cameraSpeed * deltaTime)) {
                dronePos = camera->TranslateRight(-cameraSpeed * deltaTime, true);
                updatePacket();
                cameraOrtho->TranslateRight(cameraSpeed * deltaTime, false);
            }
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
            if (!isCollided(dronePos + dir * -cameraSpeed * deltaTime)) {
                dronePos = camera->MoveForward(-cameraSpeed * deltaTime, true);
                updatePacket();
                cameraOrtho->MoveForward(-cameraSpeed * deltaTime, false);
            }
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            glm::vec3 dir = glm::normalize(glm::vec3(camera->right.x, 0, camera->right.z));
            if (!isCollided(dronePos + dir * cameraSpeed * deltaTime)) {
                dronePos = camera->TranslateRight(cameraSpeed * deltaTime, true);
                updatePacket();
                cameraOrtho->TranslateRight(-cameraSpeed * deltaTime, false);
            }
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
                glm::vec3 dir = glm::normalize(camera->up);
                if (!isCollided(dronePos + dir * -cameraSpeed * deltaTime)) {
                    dronePos = camera->TranslateUpward(-cameraSpeed * deltaTime, true);
                    updatePacket();
                }
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            glm::vec3 dir = glm::normalize(camera->up);
            if (!isCollided(dronePos + dir * -cameraSpeed * deltaTime)) {
                dronePos = camera->TranslateUpward(cameraSpeed * deltaTime, true);
                updatePacket();
            }
        }
        if (window->KeyHold(GLFW_KEY_1)) {
            droneRotOy += -deltaTime * 50;
        }
        if (window->KeyHold(GLFW_KEY_2)) {
            droneRotOy -= -deltaTime * 50;
        }
    }

}


void Tema2::OnKeyPress(int key, int mods)
{

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraHWTarget = false;

            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
            droneRotOy += -deltaX * 0.057;

            cameraOrtho->RotateFirstPerson_OY(-deltaX * sensivityOY);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
