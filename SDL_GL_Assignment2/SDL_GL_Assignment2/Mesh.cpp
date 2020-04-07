#include "Mesh.h"

Mesh::Mesh(vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndices, vec3 position)
{

	std::vector<vec3> positions;
	std::vector<vec2> texCoords;

	for (unsigned int i = 0; i < vertCount; i++)
	{
		positions.push_back(verts[i].Position);
		texCoords.push_back(verts[i].TextureCoord);
	}


	// generate normal vectors for lighting calculations
	std::vector<vec3> Normals;
	Normals.resize(vertCount);

	for (int i = 0; i < numIndices; i += 3)
	{
		vec3 vert1 = positions[indices[i]];
		vec3 vert2 = positions[indices[i + 1]];
		vec3 vert3 = positions[indices[i + 2]];

		vec3 normal = triangleNormal(vert1, vert2, vert3);
		Normals[indices[i]] += normal;
		Normals[indices[i + 1]] += normal;
		Normals[indices[i + 2]] += normal;
	}

	// generate and populate buffers
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	glGenBuffers(NUM_BUFFERS, m_vertexBufferObjects);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(POSITION_VB, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(POSITION_VB);

	// texture co-ords
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(TEXCOORD_VB, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(TEXCOORD_VB);

	// index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferObjects[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// normals
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Normals[0]), &Normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(NORMAL_VB, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(NORMAL_VB);

	// cleanup
	glBindVertexArray(0);

	m_transform = Transform();
	m_transform.setPos(position);

	m_pointCount = numIndices;
}

const void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_pointCount, GL_UNSIGNED_INT, 0);

	// safety cleanup
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}
