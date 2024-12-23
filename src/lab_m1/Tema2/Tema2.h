#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"
#define IDLE 0
#define ROTLEFT 1
#define ROTRIGHT 2
#define WALK 3
#define NOTICED 4
#define ATTACKING 5

namespace m1
{
    class CameraHW;
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();
        friend class CameraHW;
        void Init() override;
        bool isCollided(glm::vec3 drPos);

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::CameraHW* camera);
        

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void renderDrone(Mesh* block, Mesh* cube, float droneRotOy, float propRot, float deltaTimeSeconds, glm::vec3 position, implemented::CameraHW* camera);
        void renderArrow(float deltaTimeSeconds, glm::vec3 dronePos, glm::vec3 packetPos, implemented::CameraHW* camera);
        void renderDroneArrow(float deltaTimeSeconds, glm::vec3 position, implemented::CameraHW* camera);
        void updatePropRot(float deltaTime);
        void renderTerrain(float deltaTimeSeconds, glm::vec3 position, std::string shader, implemented::CameraHW* camera);
        bool isInsideCylinder(glm::vec3 pos, float radius, float height, glm::vec3 drPos);
        void renderTrees(float deltaTimeSeconds, implemented::CameraHW* camera);
        bool isInsideCone(glm::vec3 cylPos, float radius, float height, glm::vec3 drPos);
        void RenderCone(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 center, float radius, implemented::CameraHW* camera);
        bool isInsidePar(glm::vec3 cen, glm::vec3 dim, glm::vec3 drPos);
        void updatePacketGame(float deltaTimeSeconds, implemented::CameraHW* camera);
        void renderPacketGame(float deltaTimeSeconds, implemented::CameraHW* camera);
        void renderEnemies(float deltaTimeSeconds, implemented::CameraHW* camera);
        void updateTimers(float deltaTimeSeconds);
        void updatePackets(float deltaTimeSeconds);
        void renderBuildings(implemented::CameraHW* camera);
        void renderExPoint(glm::vec3 pos);
        void resetGame();
        void updatePacket();
        glm::vec3 findRandomPlace();
     protected:
        implemented::CameraHW *camera;
        implemented::CameraHW* cameraOrtho;
        implemented::CameraHW* dumbCamera;
        glm::mat4 projectionMatrix;
        bool renderCameraHWTarget;

        float FoV = RADIANS(60);
        float left = -20.0f;
        float right = 20.0f;
        float bottom = -10.0f;
        float top = 10.0f;

        glm::vec3 initPos = glm::vec3(0, 2, 0);
        glm::vec3 dronePos = initPos;
        float droneRotOy = 0;
        float propRot = 0;
        float treeRadius = 0.3;
        struct Tree{
            struct Cone {
                float height;
                float radius;
                glm::vec3 pos;
                Cone(float height, float radius, glm::vec3 pos) {
                    this->height = height;
                    this->radius = radius;
                    this->pos = pos;
                }
            };
            std::vector<Cone> cones;
            glm::vec3 pos = glm::vec3(0, 0, 0);
            int numCones = 3;
            float trunkHeight = 1;
            Tree(glm::vec3 pos, int numCones, float trunkHeight) {
                this->pos = pos;
                this->numCones = numCones;
                this->trunkHeight = trunkHeight;
                trunkHeight += 3;
                float conesHeight = 0.75 * trunkHeight;
                glm::vec3 crtPos = glm::vec3(pos.x, pos.y + 0.25 * trunkHeight, pos.z);

                float crtHeight = conesHeight - conesHeight / numCones;
                float crtWidth = trunkHeight * 0.5;
                for (int i = 0; i < numCones; i++) {
                    cones.push_back(Cone(crtHeight + crtHeight/2, crtWidth, crtPos));
                    crtPos.y += crtHeight;
                    conesHeight = conesHeight - crtHeight;
                    crtHeight = conesHeight - conesHeight / numCones;
                    crtWidth = crtWidth / 2;
                }
            }
        };
        struct Building {
            glm::vec3 pos;
            glm::vec3 dim;
            Building(glm::vec3 pos, glm::vec3 dim) {
                this->pos = pos;
                this->dim = dim;
            }
        };
        std::vector<Building> buildings;
        struct Enemy {
            glm::vec3 pos;
            glm::vec3 dimensions;
            glm::mat4 rotMatrix;
            float internalTimer;
            int crtAction;
            glm::vec3 forward;
            float isAgro;
            float atkTimer;
            float blinkInterval;
            bool isBlinking;
            bool isExploding;
            float explosionRadius;
            bool isDead;
            Enemy(glm::vec3 pos) {
                this->pos = pos;
                dimensions = glm::vec3(2, 1.6, 2);
                internalTimer = 0;
                forward = glm::vec3(0, 0, -1);
                crtAction = IDLE;
                rotMatrix = glm::mat4(1);
                isAgro = false;
                atkTimer = 0;
                blinkInterval = 0.8;
                isBlinking = false;
                explosionRadius = 0.01;
                isExploding = false;
                isDead = false;
            }
        };
        struct Packet {
            glm::vec3 pos;
            bool isFalling = true;
            bool isPicked = false;
            Packet(glm::vec3 pos) {
                this->pos = pos;
            }
        };
        float packFallSpd = 3;
        std::vector<Packet> healthPackets;
        void updateEnemyIdle(float deltaTimeSeconds, Tema2::Enemy* enemy);
        void updateEnemyAgro(float deltaTimeSeconds, Tema2::Enemy* enemy);
        float aggroDist = 16;
        float enemySpd = 2;
        float enemyRotateSpd = 1;
        int numEnemiesMax = 30;
        std::vector<Enemy> enemies;
        float enemyTimerMax = 1;
        void renderEnemy(float deltaTimeSeconds, Tema2::Enemy enemy, implemented::CameraHW* camera);
        void updateEnemy(float deltaTimeSeconds, Tema2::Enemy* enemy);

        bool areEnemiesOn = true;
        int health = 3;

        float spawnCounterEnemy = 0;
        float spawnTimeEnemy = 3;

        bool isFriendSpawned = false;
        float friendInterval = 7;
        float friendTimer = 0;
        struct FriendDr {
            float timer;
            glm::vec3 pos;
            glm::vec3 packetPos;
            bool hasPacket = true;
            bool hasAddedPckToArr = false;
            float randDistance = 0;
            FriendDr() {
                timer = 0;
                this->pos = glm::vec3(0, 0, 0);
            }
            FriendDr(glm::vec3 pos) {
                timer = 0;
                this->pos = pos;
                packetPos = pos - glm::vec3(0, 1.3, 0);
                randDistance = (rand() % 100) - 30;
            }
        };
        void updateFriend(float deltaTimeSeconds, Tema2::FriendDr* crtFriend);
        float friendSpeed = 10;
        Tema2::FriendDr crtFriend;
        std::vector<Tema2::Tree> treeList;
        void renderTree(float deltaTimeSeconds, Tree tree, implemented::CameraHW* camera);
        bool isPacketGameOn = true;
        glm::vec3 packetPos = glm::vec3(10, 0.5, 10);
        glm::vec3 packetDim = glm::vec3(1, 0.7, 0.8);
        bool isPacketPicked = false;
        float goalRadius = 3;
        glm::vec3 goalPos = glm::vec3(50, 0.5, 40);
        int packetsArrived = 0;
        glm::vec2 miniArea;
        glm::vec3 cameraOrthoPos;
    };
}   // namespace m1
