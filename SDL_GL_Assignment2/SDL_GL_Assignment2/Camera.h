#pragma once

#include "Transform.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <glew.h>


class Camera
{
public:

	Camera(float FOVy, float aspecty, float nearClipy, float farClipy) : //, vec2 pos = vec2(0, 720), vec2 dimension = { 1080, 720 }) :
		fov(FOVy),
		aspectRatio(aspecty),
		nearClip(nearClipy),
		farClip(farClipy)
		//ScreenPos(pos),
		//Dimensions(dimension)
	{
		//m_target = vec3(0);
	}

	Transform m_transform;

	mat4 GenerateViewMatrix();
	mat4 GeneratePerspectiveProjectionMatrix();

	vec3 m_target = { 1,1,1 };
	vec3 forwardTarget = { 0,0,0 };

	float xTheta = 0;
	float yTheta = 0;

	float targetDeltaX = 0;
	float targetDeltaY = 0;

	float sensitivity = 0.5f;

	//vec2 ScreenPos;
	//vec2 Dimensions;

#pragma region Getters and Setters
	vec3 getForwardVector()
	{
		return Forward;
	}

	void setForwardVector(vec3 newVector)
	{
		Forward = newVector;
	}

	vec3 getUpVector()
	{
		return Up;
	}

	void setUpVector(vec3 newVector)
	{
		Up = newVector;
	}

	vec3 getRightVector()
	{
		return Right;
	}

	void setRightVector(vec3 newVector)
	{
		Right = newVector;
	}
#pragma endregion Getters and setters

private:

	float fov;
	float aspectRatio;
	float nearClip;
	float farClip;

	vec3 Forward;
	vec3 Up;
	vec3 Right;

	mat4 PerspectiveProjection;
	mat4 ViewMatrix;
};