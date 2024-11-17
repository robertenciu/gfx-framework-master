#include "Tank.h"

#include <lab_m1/TankWars/transform2D.h>
#include "lab_m1/TankWars/obj2D.h"
#include "lab_m1/TankWars/Terrain.h"
#include <vector>

#define MOVING_OFFSET 80
#define ANGLE_OFFSET 0.66

using namespace std;

Tank::Tank(int startingX, Terrain *terrain) {
	this->coordinates.x = startingX;
	this->coordinates.y = terrain->getHeightMap()[startingX];
	this->isShooting = false;
	this->angleOfAttack = 0;
	this->terrain = terrain;
	this->health = 18;
	updateRotationAngle();
}
void Tank::updateAngleOfAttack(bool clockwiseDirection) {
	if (clockwiseDirection) {
		this->angleOfAttack -= ANGLE_OFFSET * deltaTime;
	}
	else {
		this->angleOfAttack += ANGLE_OFFSET * deltaTime;
	}
}
void Tank::update(float deltaTime) {
	this->deltaTime = deltaTime;
	updateY();
	updateRotationAngle();
}
void Tank::updateY() {
	int A_x = this->coordinates.x - int(this->coordinates.x) % terrain->getFlatness();
	int B_x = A_x + terrain->getFlatness();
	float A_y = terrain->getHeightMap()[A_x];
	float B_y = terrain->getHeightMap()[B_x];
	float t = (this->coordinates.x - A_x) / (B_x - A_x);
	this->coordinates.y = A_y + t * (B_y - A_y);
}
void Tank::updateRotationAngle() {
	int A_x = this->coordinates.x - int(this->coordinates.x) % terrain->getFlatness();
	int B_x = A_x + terrain->getFlatness();
	float A_y = terrain->getHeightMap()[A_x];
	float B_y = terrain->getHeightMap()[B_x];
	rotationAngle = atan2((B_y - A_y), (B_x - A_x));
}
void Tank::moveLeft() {
	const int marginLeft = 15;
	if (this->coordinates.x < marginLeft) {
		return;
	}

	this->coordinates.x -= MOVING_OFFSET * deltaTime;
	this->updateY();
	this->updateRotationAngle();
}
void Tank::moveRight() {
	const int marginRight = terrain->getWidth() - 15;
	if (this->coordinates.x > marginRight) {
		return;
	}

	this->coordinates.x += MOVING_OFFSET * deltaTime;
	this->updateY();
	this->updateRotationAngle();
}

glm::mat3 Tank::getBaseModel() {
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(this->coordinates.x, this->coordinates.y);
	modelMatrix *= transform2D::Rotate(rotationAngle);
	return modelMatrix;
}
glm::mat3 Tank::getGunModel() {
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(this->coordinates.x, this->coordinates.y);
	modelMatrix *= transform2D::Rotate(rotationAngle);
	modelMatrix *= transform2D::Translate(0, 25);
	modelMatrix *= transform2D::Rotate(angleOfAttack - rotationAngle);
	modelMatrix *= transform2D::Scale(1, 10 / 1.5);
	return modelMatrix;
}
glm::mat3 Tank::getBombModel() {
	int posBombaX = bomb.getCoordinates().x;
	int posBombaY = bomb.getCoordinates().y;
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(posBombaX, posBombaY);
	modelMatrix *= transform2D::Rotate(rotationAngle);
	modelMatrix *= transform2D::Translate(0, 25);
	modelMatrix *= transform2D::Rotate(bomb.getRotationAngle());
	return modelMatrix;
}
glm::mat3 Tank::getHealthModel()
{
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(this->coordinates.x, this->coordinates.y + 40);
	modelMatrix *= transform2D::Rotate(rotationAngle);
	modelMatrix *= transform2D::Translate(0, 10);
	modelMatrix *= transform2D::Rotate(-rotationAngle);
	modelMatrix *= transform2D::Scale(float(health) / 2, 1);
	return modelMatrix;
}
vector<glm::mat3> Tank::trajectoryModelGenerator() {
	vector<glm::mat3> models;

	glm::vec2 trajectory = glm::vec2(this->coordinates.x, this->coordinates.y);
	float gravity = Bomb::gravity;
	float magnitude = Bomb::power;
	float staticDT = 0.3;
	float trajectoryLength = 8;

	glm::vec2 currentVelocity = glm::vec2(magnitude * glm::cos(angleOfAttack + glm::pi<float>() / 2),
		magnitude * glm::sin(angleOfAttack + glm::pi<float>() / 2));
	for (float t = 0.0f; t < trajectoryLength; t += staticDT) {
		glm::mat3 modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(trajectory.x, trajectory.y);
		modelMatrix *= transform2D::Rotate(rotationAngle);
		modelMatrix *= transform2D::Translate(0, 25);
		models.push_back(modelMatrix);

		trajectory.x += currentVelocity.x * staticDT;
		trajectory.y += currentVelocity.y * staticDT;
		currentVelocity.y -= gravity * staticDT;

		if (trajectory.y < terrain->getHeightMap()[int(trajectory.x)] - 25) {
			break;
		}
		if (trajectory.x < 0 || trajectory.x > terrain->getWidth()) {
			break;
		}
	}

	return models;
}
void Tank::startShooting() {
	if (!isShooting) {
		isShooting = true;
		bomb.setStartingPoint(this->coordinates.x, this->coordinates.y);
		bomb.setAngleOfAttack(this->angleOfAttack);
		bomb.resetVelocity();
	}
}
void Tank::updateShooting(Tank *otherTank) {
	if (!isShooting) {
		return;
	}
	bool hittingSecondPlayer = false;
	bomb.updateBomb(deltaTime, &isShooting, terrain, otherTank);
}
void Tank::getDamage() {
	if (health > 0) {
		health -= 2;
	}
}
bool Tank::isTankShooting() {
	return isShooting;
}
int Tank::getHealth()
{
	return health;
}
Point Tank::getCoordinates() {
	return coordinates;
}