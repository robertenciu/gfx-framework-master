#pragma once
#include "components/simple_scene.h"

#include <cstdlib>
#include <lab_m1/TankWars/transform2D.h>

class Terrain : public gfxc::SimpleScene {
public:
	float *heightMapGenerator();
private:
	float shapeFunction(float x);

protected:
};