#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <glew.h>
#include "Camera.h"
#include "Transform.h"
#include "LightBase.h"

#include "Time.h"

enum UNIFORMS
{
	MODEL_U,
	PROJECTION_U,
	VIEW_U,
	FRAG_LIGHTCOLOR_U,
	FRAG_LIGHTPOS_U,
	FRAG_LIGHTFALLOFF_U,
	FRAG_LIGHTCOLOR_U2,
	FRAG_LIGHTPOS_U2,
	FRAG_LIGHTFALLOFF_U2,
	FRAG_CAMERAPOS_U,
	TIME_U,
	NUMBER_UNIFORMS
};

class Shader
{
public:
	std::string readFile(const std::string &filePath);

	Shader(const std::string vertFilepath, const std::string fragFilePath, Camera& _cam);
	~Shader();

	void Bind();

	void Update(Transform* transform, LightBase& light, LightBase* light2 = nullptr);

	GLuint getProgram()
	{
		return program;
	}

private:
	std::ifstream shaderFile;
	Camera* cam;

	GLuint vertShader;
	GLuint fragShader;

	GLuint program;

	GLuint uniformLocation[NUMBER_UNIFORMS];
};