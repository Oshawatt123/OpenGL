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
	Mesh(vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndices);

	const void Draw();
	~Mesh();

private:
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		TANGENT_VB,
		BITANGENT_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint m_vertexBufferObjects[NUM_BUFFERS];
	GLuint m_vertexArrayObject = 0;

	GLuint m_pointCount;
	
	void CalculateTangentBiTangent(vertex* vertices, unsigned int vertCount, unsigned int* indices, unsigned int numIndices);
};