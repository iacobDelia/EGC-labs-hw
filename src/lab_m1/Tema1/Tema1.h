#pragma once

#include "lab_m1/Tema1/Projectile.h"
#include "components/simple_scene.h"


namespace m1
{
    class Projectile;
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;
        friend class Projectile;
    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        float constrFunc(float X);
        void addToList(Mesh* mesh);
        void renderTerrain();
        void renderTank(Mesh* tankMesh, Mesh* cannonMesh, Mesh* trajectoryMesh, int tankPosInd, float deltaTimeSeconds, float rotRate, int health);
        void renderProjectiles(float deltaTimeSeconds);
        void landSlide(float deltaTimeSeconds);
        int nearestIndex(float X);
        void explodeTerrain(int index);
        void renderTrajectory(float deltaTimeSeconds, int tankIndPos, float rotRate);
        void renderStars(float deltaTimeSeconds);
        void renderOneStar(bool state, glm::vec3 pos);
        void updateExplosions(float deltaTimeSeconds);
        void updateEnemyTank(float deltaTimeSeconds);
    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;

        // TODO(student): If you need any other class variables, define them here.
        std::unordered_map<std::string, Mesh*> squareList;
        float numbPoints = 500;
        float rotRate1 = 0;
        float rotRate2 = 0;
        float trajectory1 = 1.5;
        float trajectory2 = 1.5;

        float tank1PosInd;
        float tankSpd = 50;
        float cannonSpd = 2;
        int Rint = 30;

        float tank2PosInd;
        int tank1Life = 5;
        int tank2Life = 5;
        float tank2Timer = 0;
        int starAnimTimer = 0;
        bool starAnimCondition = true;
        int tank2Move = 1;
        bool hasLaunched = false;
        glm::vec3 tankCentre;
        std::vector<glm::vec3> pointList;
        bool manualControl = false;
        //std::vector<int> indExplList;
        std::vector<Projectile*> projectileList;
        struct Explosion {
            int ind;
            float timer;
            bool hasExploded;
            float scale;
            Explosion(int where, float howLong) {
                ind = where;
                timer = 0;
                hasExploded = false;
                scale = 1;
            }
        };
        std::vector<Explosion> expList;
    };
}   // namespace m1
