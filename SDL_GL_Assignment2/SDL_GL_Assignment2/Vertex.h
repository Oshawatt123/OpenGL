#pragma once

struct vertex
{
	glm::vec3 Position;
	glm::vec2 TextureCoord;

	glm::vec3 tangent;
	glm::vec3 biTangent;
	glm::vec3 normal;

	vertex()
	{
		this->Position = glm::vec3(0);
		this->TextureCoord = glm::vec2(0);

		this->tangent = glm::vec3(0);
		this->biTangent = glm::vec3(0);
	}

	vertex(const float i_x, const float i_y, const float i_z)
	{
		this->Position.x = i_x;
		this->Position.y = i_y;
		this->Position.z= i_z;

		TextureCoord = { 0,0 };

		tangent = glm::vec3(0);
		biTangent = glm::vec3(0);
	}

	vertex(const glm::vec3 position)
	{
		vertex(position.x, position.y, position.z);
	}

	vertex(const glm::vec3 position, const glm::vec2 texCoord)
	{
		this->Position = position;
		this->TextureCoord = texCoord;

		this->tangent = glm::vec3(0);
		this->biTangent = glm::vec3(0);
	}

	vertex(const vertex& vert)
	{
		this->Position = vert.Position;
		TextureCoord = vert.TextureCoord;

		tangent = glm::vec3(0);
		biTangent = glm::vec3(0);
	}
};