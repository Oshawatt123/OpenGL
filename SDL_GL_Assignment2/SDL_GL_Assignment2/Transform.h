#pragma once

#include <glm.hpp>
#include <gtx\transform.hpp>

using namespace glm;

class Transform
{
public:
	Transform(const vec3& pos = vec3(),
		const vec3& rot = vec3(0, 0, 2),
		const vec3& scale = vec3(1, 1, 1)) :
		m_position(pos),
		m_rotation(rot),
		m_scale(scale)
	{}

	~Transform()
	{}

	void Translate(float x = 0, float y = 0, float z = 0)
	{
		m_position.x += x;
		m_position.y += y;
		m_position.z += z;
	}

	void Translate(vec3 translation)
	{
		m_position += translation;
	}

#pragma region Getters and Setters
	vec3 getPos()
	{
		return m_position;
	}

	void setPos(vec3 position)
	{
		m_position = position;
	}

	vec3 getRotation()
	{
		return m_rotation;
	}

	void setRotation(vec3 rotation)
	{
		m_rotation = rotation;
	}

	vec3 getScale()
	{
		return m_scale;
	}

	void setScale(vec3 scale)
	{
		m_scale = scale;
	}
#pragma endregion Getters and setters

	inline mat4 GetModel() const
	{
		mat4 posMatrix = translate(m_position);

		mat4 rotXmatrix = rotate(m_rotation.x, vec3(1, 0, 0));
		mat4 rotYMatrix = rotate(m_rotation.y, vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(m_rotation.z, vec3(0, 0, 1));
		mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXmatrix; // does X * Y * Z because operator overloading is a function when overloaded on a class so is really Z * ( Y * (x)) so the brackets are done first

		mat4 scaleMatrix = scale(m_scale);

		return posMatrix * rotMatrix * scaleMatrix;

	}

private:
	vec3 m_position;
	vec3 m_rotation;
	vec3 m_scale;

};