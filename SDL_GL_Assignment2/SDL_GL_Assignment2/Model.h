#pragma once
#include <string>

#include "OBJLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "LightBase.h"

class Model
{
public:
	Model(std::string ModelLoc, std::string ModelName);

	void Draw(Shader& program, LightBase& light, Transform& transform);

private:

	std::string AmbientLoc;
	std::string DiffuseLoc;
	std::string SpecLoc;
	std::string NormalLoc;

	GLuint AmbTexID;
	GLuint DiffTexID;
	GLuint SpecTexID;
	GLuint NormTexID;

	std::vector<vertex> vertices;
	std::vector<uint> Indices;

	Mesh* mesh;


};