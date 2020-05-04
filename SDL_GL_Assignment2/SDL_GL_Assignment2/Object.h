#pragma once
#include "Transform.h"
#include "Model.h"
#include "Shader.h"
#include "LightBase.h"

#include <string>

class Object
{
public:
	Object(std::string name = "Barry");
	Object(std::string ModelLoc, std::string modelName, std::string name = "Barry");

	std::string ObjectName;

	void Draw(Shader& program, LightBase& light, LightBase* light2 = nullptr);

	void SetScale(float newScale)
	{
		transform.setScale(glm::vec3(newScale, newScale, newScale));
	}

private:
	Model* model;

	Transform transform;

	void Init(std::string ModelLoc, std::string modelName, std::string name = "Barry");
};