#include "Mesh.h"

Mesh::Mesh(vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndices, vec3 position)
{
	CalculateTangentBiTangent(verts, vertCount, indices, numIndices);

	std::vector<vec3> positions;
	std::vector<vec2> texCoords;
	std::vector<vec3> tangents;
	std::vector<vec3> biTangents;

	for (unsigned int i = 0; i < vertCount; i++)
	{
		positions.push_back(verts[i].Position);
		texCoords.push_back(verts[i].TextureCoord);
		tangents.push_back(verts[i].tangent);
		biTangents.push_back(verts[i].biTangent);
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

	// tangents
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[TANGENT_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(tangents[0]), &tangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(TANGENT_VB, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(TANGENT_VB);

	// bitangents
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjects[BITANGENT_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(biTangents[0]), &biTangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(BITANGENT_VB, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(BITANGENT_VB);

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

void Mesh::CalculateTangentBiTangent(vertex* vertices, unsigned int vertCount, unsigned int* indices, unsigned int numIndices)
{
	// calculate tangent and bitangent
	for (unsigned int i = 0; i < numIndices; i += 3)
	{
		vertex v0 = vertices[indices[i]];
		vertex v1 = vertices[indices[i+1]];
		vertex v2 = vertices[indices[i+2]];

		vec3 edge1 = v1.Position - v0.Position;
		vec3 edge2 = v2.Position - v1.Position;

		GLfloat deltaU1 = v1.TextureCoord.x - v0.TextureCoord.x;
		GLfloat deltaV1 = v1.TextureCoord.y - v0.TextureCoord.y;
		GLfloat deltaU2 = v2.TextureCoord.x - v0.TextureCoord.x;
		GLfloat deltaV2 = v2.TextureCoord.y - v0.TextureCoord.y;

		GLfloat f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

		vec3 tangent;
		vec3 biTangent;

		tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
		tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
		tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

		biTangent.x = f * (-deltaU2 * edge1.x + deltaU1 * edge2.x);
		biTangent.y = f * (-deltaU2 * edge1.y + deltaU1 * edge2.y);
		biTangent.z = f * (-deltaU2 * edge1.z + deltaU1 * edge2.z);

		v0.tangent += tangent;
		v1.tangent += tangent;
		v2.tangent += tangent;

		v0.biTangent += biTangent;
		v1.biTangent += biTangent;
		v2.biTangent += biTangent;

		vertices[indices[i]] = v0;
		vertices[indices[i+1]] = v1;
		vertices[indices[i+2]] = v2;
	}

	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertices[i].tangent = normalize(vertices[i].tangent);
		vertices[i].biTangent = normalize(vertices[i].biTangent);
	}
}
