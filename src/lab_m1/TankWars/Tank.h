#pragma once

#include <lab_m1/TankWars/transform2D.h>
#include <lab_m1/TankWars/Point.h>
#include <lab_m1/TankWars/Terrain.h>
#include <lab_m1/TankWars/Bomb.h>
#include <vector>

class Tank {
public:
	Tank(int startingX, Terrain *terrain);
	~Tank();

	void update(float deltaTime);
	void updateAngleOfAttack(bool clockwiseDirection);
	void moveLeft();
	void moveRight();
	void startShooting();
	void updateShooting(Tank *otherTank);
	void getDamage();
	bool isTankShooting();
	int getHealth();
	Point getCoordinates();
	std::vector<glm::mat3> trajectoryModelGenerator();
	glm::mat3 getBaseModel();
	glm::mat3 getGunModel();
	glm::mat3 getBombModel();
	glm::mat3 getHealthModel();
private:
	Point coordinates;
	Bomb bomb;
	Terrain* terrain;
	bool isShooting;
	int health;
	float angleOfAttack;
	float rotationAngle;
	float deltaTime;

	void updateRotationAngle();
	void updateY();
};