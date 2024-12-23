#pragma once

#include "utils/glm_utils.h"

#include "utils/math_utils.h"


namespace implemented
{
    //class Tema2;
    class CameraHW
    {
     public:
         float camMoveSpd = 2.0f;
         
        CameraHW()
        {
            position    = glm::vec3(0, 2, 6);
            dronePos = glm::vec3(0, 2, 0);
            droneRot = glm::vec3(0, 0, 0);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            distanceToTarget = 6;
        }

        CameraHW(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~CameraHW()
        { }

        // Update camera
        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            this->position = position;
            forward     = glm::normalize(center - position);
            right       = glm::cross(forward, up);
            this->up    = glm::cross(right, forward);
        }

        glm::vec3 MoveForward(float distance, bool moveDrone)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            
            position += dir * distance;
            if(moveDrone)
                dronePos += dir * distance;
            
            return dronePos;
        }

        glm::vec3 TranslateForward(float distance, bool cameFromRot, bool moveDrone)
        {
            glm::vec3 dir = glm::normalize(forward);
            position += dir * distance;
            if(moveDrone)
                if(cameFromRot)
                    dronePos += dir * distance;
            return dronePos;

        }

        glm::vec3 TranslateUpward(float distance, bool moveDrone)
        {
            glm::vec3 dir = glm::normalize(up);
            if(moveDrone)
                if ((dronePos + dir * distance).y > 0 && (dronePos + dir * distance).y < 10) {
                    dronePos += dir * distance;
                    position += dir * distance;
                }

            return dronePos;
        }

        glm::vec3 TranslateRight(float distance, bool moveDrone)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
            position += dir * distance;
            if(moveDrone)
                dronePos += dir * distance;
            return dronePos;

        }

        void RotateFirstPerson_OX(float angle)
        {
            forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 1)));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OY(float angle)
        {
            forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1)));
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1)));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OZ(float angle)
        {
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(right, 1)));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget, false, true);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget, false, true);
        }

        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget, false, true);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget, false, true);
        }

        glm::vec3 RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget, false, true);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget, false, true);
        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

     public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
        glm::vec3 dronePos;
        glm::vec3 droneRot;
    };
}   // namespace implemented
