#include "OBJLoader.h"

std::vector<vertex> OBJLoader::LoadOBJ(const std::string& FolderLoc, const std::string& OBJFileName, std::string& AmbientLoc, std::string& DiffuseLoc, std::string& SpecularLoc, std::string& NormalLoc, std::vector<glm::uint>& indices)
{
	std::string line;

	std::string MatLibName;
	std::string MeshMaterial;
	std::string MeshName;

	std::vector<glm::vec3> VertPositions;
	std::vector<glm::vec3> VertNormals;
	std::vector<glm::vec3> VertTexCoords;

	std::vector<vertex> FinalVerts;


	std::ifstream file;
	std::string fileLoc = FolderLoc + "/" + OBJFileName;
	const char* fileLocChar = fileLoc.c_str();
	file.open(fileLocChar, std::ifstream::in);

	if (file.is_open())
	{
		while (file.good())
		{
			std::getline(file, line);
			// # means comments, so if we find a comment, ignore it
			if (line[0] != '#')
			{
				std::string identifier = line.substr(0, line.find(' '));

				// parse data based on identifier
				if (identifier == "mtllib") // material library location
					MatLibName = READ_EOL_PLUS1;

				else if (identifier == "o") // object name
					MeshName = READ_EOL;

				else if (identifier == "v") // vertex information
				{
					std::string VertPosValues = READ_EOL;
					float x, y, z;
					sscanf_s(VertPosValues.c_str(), "%f %f %f", &x, &y, &z);
					VertPositions.push_back(glm::vec3(x, y, z));
				}
				else if (identifier == "vn")
				{
					std::string VertNormValues = READ_EOL;
					float x, y, z;
					sscanf_s(VertNormValues.c_str(), "%f %f %f", &x, &y, &z);
					VertNormals.push_back(glm::vec3(x, y, z));
				}
				else if (identifier == "vt")
				{
					std::string VertTexValues = READ_EOL;
					float x, y, z;
					sscanf_s(VertTexValues.c_str(), "%f %f %f", &x, &y, &z);
					VertTexCoords.push_back(glm::vec3(x, y, z));
				}
				else if (identifier == "usemtl")
				{
					MeshMaterial = READ_EOL_PLUS1;
					LoadMaterial(FolderLoc + "/" + MatLibName, AmbientLoc, DiffuseLoc, SpecularLoc, NormalLoc);
				}
				else if (identifier == "f") // geometry face list (indices)
				{
					std::string faceValues = READ_EOL;

					vertex vertsInFace[3];

					unsigned int TempPos[3], TempTexCoords[3], TempNormals[3];

					sscanf_s(faceValues.c_str(), " %d/%d/%d %d/%d/%d %d/%d/%d",
						&TempPos[0], &TempTexCoords[0], &TempNormals[0],
						&TempPos[1], &TempTexCoords[1], &TempNormals[1],
						&TempPos[2], &TempTexCoords[2], &TempNormals[2]);

					vertsInFace[0].Position = VertPositions[TempPos[0] - 1];
					vertsInFace[0].TextureCoord = VertTexCoords[TempTexCoords[0] - 1];
					vertsInFace[0].normal = VertNormals[TempNormals[0] - 1];

					vertsInFace[1].Position = VertPositions[TempPos[1] - 1];
					vertsInFace[1].TextureCoord = VertTexCoords[TempTexCoords[1] - 1];
					vertsInFace[1].normal = VertNormals[TempNormals[1] - 1];

					vertsInFace[2].Position = VertPositions[TempPos[2] - 1];
					vertsInFace[2].TextureCoord = VertTexCoords[TempTexCoords[2] - 1];
					vertsInFace[2].normal = VertNormals[TempNormals[2] - 1];

					FinalVerts.push_back(vertsInFace[0]);
					FinalVerts.push_back(vertsInFace[1]);
					FinalVerts.push_back(vertsInFace[2]);
				}
			}
		}
	}
	else
	{
		std::cerr << "Unable to load OBJ file " << fileLoc << std::endl;
	}

	file.close();

	for (int i = 0; i < FinalVerts.size(); i++)
	{
		indices.push_back(i);
	}

	return FinalVerts;
}

void OBJLoader::LoadMaterial(const std::string& MatLibLoc, std::string& AmbientLoc, std::string& DiffuseLoc, std::string& SpecularLoc, std::string& NormalLoc)
{
	std::ifstream file;
	const char* fileNameChar = MatLibLoc.c_str();
	file.open(fileNameChar, std::ifstream::in);

	std::string line;
	std::string MatName;

	if (file.is_open())
	{
		while (file.good())
		{
			std::getline(file, line);
			if (line[0] != '#') // check for comment
			{
				std::string identifier = line.substr(0, line.find(' '));
				if (strstr(identifier.c_str(), "newmtl")) // material
					MatName = READ_EOL_PLUS1;

				else if (strstr(identifier.c_str(), "map_Ka"))
					AmbientLoc = READ_EOL_PLUS1;

				else if (strstr(identifier.c_str(), "map_Kd"))
					DiffuseLoc = READ_EOL_PLUS1;

				else if (strstr(identifier.c_str(), "map_Ks"))
					SpecularLoc = READ_EOL_PLUS1;

				else if (strstr(identifier.c_str(), "map_bump"))
					NormalLoc = READ_EOL_PLUS1;
			}
		}
	}
	else
	{
		std::cerr << "Unable to load material file: " << MatLibLoc << std::endl;
	}
	file.close();
}
