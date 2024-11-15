#pragma once
#include "components/simple_scene.h"

#include <cstdlib>
#include <lab_m1/TankWars/transform2D.h>

class Terrain {

public:
	Terrain(int flatness, int width);
	~Terrain();
	std::vector<glm::mat3> heightMapModelGenerator();
	float* getHeightMap();
	float* setHeightMap();
private:
	float* heightMapGenerator();
	float shapeFunction(float x);
	float *heightMap;
	int flatness;
	int width;

protected:
};