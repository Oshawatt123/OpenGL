#pragma once

#include <glew.h>

#include "Transform.h"
#include "Camera.h"

struct GL_LightData
{
	vec3 position;
	float falloff;
	vec3 color;
};

class LightBase
{
public:
	LightBase();
	~LightBase();

	void Update();
	void Draw(Camera* cam);

	Transform* getTransform();

	GL_LightData lightData;

private:
	Transform transform;
};