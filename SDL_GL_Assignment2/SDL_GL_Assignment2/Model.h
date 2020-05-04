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

	void Draw(Shader& program, Transform& transform, LightBase& light, LightBase* light2 = nullptr);

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