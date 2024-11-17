#pragma once

#include <cstdlib>
#include <lab_m1/TankWars/transform2D.h>
#include <vector>

class Terrain {

public:
	Terrain(int flatness, int width);
	~Terrain();
	std::vector<glm::mat3> heightMapModelGenerator();
	float* getHeightMap();
	int getFlatness();
	int getWidth();
private:
	float* heightMapGenerator();
	float shapeFunction(float x);
	float* heightMap;
	int flatness;
	int width;

protected:
};