#include "Model.h"

Model::Model(std::string ModelLoc, std::string ModelName)
{
	vertices = OBJLoader::LoadOBJ(ModelLoc, ModelName, AmbientLoc, DiffuseLoc, SpecLoc, NormalLoc, Indices);

	mesh = new Mesh(&vertices[0], vertices.size(), &Indices[0], Indices.size());

	AmbTexID = OBJLoader::LoadTexture(ModelLoc + "/" + AmbientLoc);
	DiffTexID = OBJLoader::LoadTexture(ModelLoc + "/" + DiffuseLoc);
	SpecTexID = OBJLoader::LoadTexture(ModelLoc + "/" + SpecLoc);
	NormTexID = OBJLoader::LoadTexture(ModelLoc + "/" + NormalLoc);
}

void Model::Draw(Shader& program, Transform& transform, LightBase& light, LightBase* light2)
{
	program.Bind();

	GLuint textureLoc;

	glActiveTexture(GL_TEXTURE0);
	textureLoc = glGetUniformLocation(program.getProgram(), "texture_diffuse");
	glUniform1i(textureLoc, 0); // 0 for location 0
	glBindTexture(GL_TEXTURE_2D, DiffTexID);

	glActiveTexture(GL_TEXTURE1);
	textureLoc = glGetUniformLocation(program.getProgram(), "texture_normal");
	glUniform1i(textureLoc, 1); // 1 for location 1
	glBindTexture(GL_TEXTURE_2D, NormTexID);

	program.Update(&transform, light, light2);

	mesh->Draw();
}