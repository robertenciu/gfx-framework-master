#include "Bomb.h"
#include "lab_m1/TankWars/Tank.h"

const float Bomb::power = 60.0f;
const float Bomb::gravity = 9.81f;
const int Bomb::explosionRadius = 50;

using namespace std;

void Bomb::setStartingPoint(float x, float y) {
	this->coordinates.x = x;
	this->coordinates.y = y;
}
void Bomb::setAngleOfAttack(float angle) {
	this->angleOfAttack = angle;
}
void Bomb::resetVelocity() {
	this->velocity = glm::vec2(power * glm::cos(angleOfAttack + glm::pi<float>() / 2),
		power * glm::sin(angleOfAttack + glm::pi<float>() / 2));
	this->rotationAngle = 0;
}
void Bomb::updateBomb(float deltaTime, bool *stillShooting, Terrain *terrain, Tank *otherTank) {
	this->rotationAngle += 0.1;
	this->coordinates.x += this->velocity.x * deltaTime * 10;
	this->coordinates.y += this->velocity.y * deltaTime * 10;
	this->velocity.y -= gravity * deltaTime * 10;
	float* heightMap = terrain->getHeightMap();

	if (coordinates.x > terrain->getWidth() - explosionRadius || coordinates.x < explosionRadius) {
		*stillShooting = false;
		return;
	}

	Point tankCoordinates = otherTank->getCoordinates();
	if (coordinates.x > tankCoordinates.x - 35 && coordinates.x < tankCoordinates.x + 35) {
		if (coordinates.y >= tankCoordinates.y && coordinates.y < tankCoordinates.y + 25) {
			otherTank->getDamage();
			*stillShooting = false;
			return;
		}
	}

	if (coordinates.y < heightMap[int(coordinates.x)] - 25) {
		*stillShooting = false;
		this->velocity.y += gravity * deltaTime * 10;
		this->coordinates.x -= this->velocity.x * deltaTime * 10;
		this->coordinates.y -= this->velocity.y * deltaTime * 10;
		int radius = explosionRadius;
		for (size_t i = 0; i < radius; i++) {
			float leftSide = heightMap[int(coordinates.x)]
				- glm::sqrt(radius * radius - (i - radius) * (i - radius));
			float rightSide = heightMap[int(coordinates.x)]
				- glm::sqrt(radius * radius - (radius - 1 - i) * (radius - 1 - i));
			if (leftSide < 0) {
				leftSide = 0;
			}
			if (rightSide < 0) {
				rightSide = 0;
			}
			if (heightMap[int(coordinates.x) - radius + i] > leftSide) {
				heightMap[int(coordinates.x) - radius + i] = leftSide;
			}
			if (heightMap[int(coordinates.x) + radius - 1 - i] > rightSide) {
				heightMap[int(coordinates.x) + radius - 1 - i] = rightSide;
			}
		}
	}
}
Point Bomb::getCoordinates() {
	return coordinates;
}
float Bomb::getRotationAngle() {
	return rotationAngle;
}