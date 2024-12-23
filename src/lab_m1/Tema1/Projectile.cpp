#pragma once
#include <lab_m1/Tema1/Projectile.h>


using namespace std;
using namespace m1;


Projectile::Projectile(int tankPosInd, int own, Tema1* tema1, float trajectory) {
	modelMatrix = glm::mat3(1);
	// find where exactly to spawn the projectile
	glm::vec3 V = tema1->pointList[tankPosInd + 1] - tema1->pointList[tankPosInd];
	glm::vec3 mid = glm::vec3(abs(tema1->pointList[tankPosInd + 1].x + tema1->pointList[tankPosInd].x) / 2,
		abs(tema1->pointList[tankPosInd + 1].y + tema1->pointList[tankPosInd].y) / 2, 0);

	float alph = atan2(V.y, V.x);
	glm::mat3 turPos = transform2DNew::Translate(0, 20.0f);
	glm::mat3 tankTrans = transform2DNew::Translate(mid.x, mid.y) * transform2DNew::Rotate(alph);
	glm::mat3 turretTrans = tankTrans * turPos;

	// apply the transformations to the coordinates
	glm::vec3 turPlaceLocal = glm::vec3(0, 20, 1);
	glm::vec3 turPlaceGlobal = turretTrans * turPlaceLocal;

	P = glm::vec2(turPlaceGlobal.x, turPlaceGlobal.y);
	float direction = 1;

	v = glm::vec2(cos(trajectory), sin(trajectory)) * glm::vec2(300, 300);
	g = glm::vec2(0, 300);
	modelMatrix *= transform2DNew::Translate(turPlaceGlobal.x, turPlaceGlobal.y);
	owner = own;
}
void Projectile::updateProjectile(float deltaTimeSeconds, Tema1* tema1) {

	P = P + v * deltaTimeSeconds;
	v = v - g * deltaTimeSeconds;
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2DNew::Translate(P.x, P.y);

	// check if its bounds
	if (P.y > 0 && P.x < tema1->pointList[tema1->pointList.size() - 1].x && P.x > 0) {
		// collision with terrain
		int nearestIndex = tema1->nearestIndex(P.x);
		if (P.y - tema1->pointList[nearestIndex].y < 5) {
			//tema1->explodeTerrain(nearestIndex);
			Tema1::Explosion newExp(nearestIndex, 0);
			tema1->expList.push_back(newExp);
			//tema1->indExplList.push_back(nearestIndex);
			eraseProjectile(this, tema1);
		}
		// collision with tanks
		glm::vec3 tank1Poz = tema1->pointList[tema1->tank1PosInd];
		glm::vec3 tank2Poz = tema1->pointList[tema1->tank2PosInd];

		float distFromTank1 = sqrt((P.x - tank1Poz.x) * (P.x - tank1Poz.x) + (P.y - tank1Poz.y) * (P.y - tank1Poz.y));
		float distFromTank2 = sqrt((P.x - tank2Poz.x) * (P.x - tank2Poz.x) + (P.y - tank2Poz.y) * (P.y - tank2Poz.y));
		if (distFromTank1 < 40 && owner == 2) {
			tema1->tank1Life--;
			eraseProjectile(this, tema1);
		}
		if (distFromTank2 < 40 && owner == 1) {
			tema1->tank2Life--;
			eraseProjectile(this, tema1);
		}
	}
}

void Projectile::eraseProjectile(Projectile* projectile, Tema1* tema1) {
	vector<Projectile*>::iterator position = find(tema1->projectileList.begin(), tema1->projectileList.end(), projectile);

	if (position != tema1->projectileList.end())
		tema1->projectileList.erase(position);
}