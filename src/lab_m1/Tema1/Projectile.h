#pragma once
#include <lab_m1/Tema1/Tema1.h>
#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2DNew.h"
#include "lab_m1/Tema1/object2D.h"

namespace m1 {
	class Tema1;
	class Projectile{
		
	public:
		Projectile(int tankPosInd, int own, Tema1* tema1, float trajectory);
		// returns index that should be exploded if its the case
		// returns -1 if no index should be exploded
		void updateProjectile(float deltaTimeSeconds, Tema1* tema1);
		glm::mat3 modelMatrix;
		glm::vec2 v;
		glm::vec2 P;
		glm::vec2 g;
		int owner;
	private:
		void eraseProjectile(Projectile* projectile, Tema1* tema1);
	};
}

