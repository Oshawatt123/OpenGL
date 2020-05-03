#include "Object.h"

Object::Object(std::string name)
{
	Init("../Models", "blocks_01.obj", name);
}

Object::Object(std::string ModelLoc, std::string modelName, std::string name)
{
	Init(ModelLoc, modelName, name);
}

void Object::Draw(Shader& program, LightBase& light)
{
	model->Draw(program, light, transform);
}

void Object::Init(std::string ModelLoc, std::string modelName, std::string name)
{
	model = new Model(ModelLoc, modelName);
	ObjectName = name;
}