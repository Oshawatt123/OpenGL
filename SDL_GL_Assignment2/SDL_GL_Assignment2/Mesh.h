#pragma once

#include <vector>

#include <glm.hpp>
#include <gtx/normal.hpp>
#include <glew.h>

#include "Transform.h"
#include "Vertex.h"

class Mesh
{
public:
	Mesh(vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndices, vec3 position = vec3(0,0,0));

	const void Draw();
	~Mesh();

	Transform m_transform;

private:
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint m_vertexBufferObjects[NUM_BUFFERS];
	GLuint m_vertexArrayObject = 0;

	GLuint m_pointCount;
};