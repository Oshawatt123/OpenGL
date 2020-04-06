#pragma once

struct vertex
{
	vec3 Position;
	vec2 TextureCoord;

	vertex(const float i_x, const float i_y, const float i_z)
	{
		this->Position.x = i_x;
		this->Position.y = i_y;
		this->Position.z= i_z;

		TextureCoord = { 0,0 };
	}

	vertex(const vec3 position)
	{
		vertex(position.x, position.y, position.z);
	}

	vertex(const vec3 position, const vec2 texCoord)
	{
		this->Position = position;
		this->TextureCoord = texCoord;
	}
};