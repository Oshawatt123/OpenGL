#pragma once

#include <glew.h>

#include "Transform.h"
#include "Camera.h"

class LightBase
{
public:
	LightBase();
	~LightBase();

	void Draw(Camera* cam);

	Transform* getTransform();

	vec3 getColor();

private:
	Transform transform;

	vec3 color;
};

