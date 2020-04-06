#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <glew.h>
#include "Camera.h"
#include "Transform.h"

enum UNIFORMS
{
	MODEL_U,
	PROJECTION_U,
	VIEW_U,
	NUMBER_UNIFORMS
};

class Shader
{
public:
	std::string readFile(const std::string &filePath);

	Shader(const std::string vertFilepath, const std::string fragFilePath, Camera& _cam);
	~Shader();

	void Bind();

	void Update(Transform* transform);

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