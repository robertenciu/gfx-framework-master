#pragma once
#include <lab_m1/TankWars/transform2D.h>
#include <lab_m1/TankWars/Point.h>
#include <lab_m1/TankWars/Terrain.h>

class Tank;

class Bomb {

public:
	static const float power;
	static const float gravity;
	static const int explosionRadius;
	Point getCoordinates();
	float getRotationAngle();
	void setStartingPoint(float x, float y);
	void setAngleOfAttack(float angle);
	void resetVelocity();
	void updateBomb(float deltaTime, bool *stillShooting, Terrain *terrain, Tank *otherTank);
private:
	Terrain* terrain;
	Point coordinates;
	glm::vec2 velocity;
	float angleOfAttack;
	float rotationAngle;
protected:
};