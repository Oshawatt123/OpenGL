#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include "glm.hpp"
#include "Vertex.h"

#define READ_EOL_PLUS1 line.substr(line.find(' ') + 1, line.find('/n'))
#define READ_EOL line.substr(line.find(' '), line.find('/n'))

class OBJLoader
{
public:
	static std::vector<vertex> LoadOBJ(const std::string& FolderLoc,
		const std::string& OBJFileName, std::string& AmbientLoc,
		std::string& DiffuseLoc, std::string& SpecularLoc,
		std::string& NormalLoc, std::vector<glm::uint>& indices);

	static void LoadMaterial(const std::string& MatLibLoc,
		std::string& AmbientLoc, std::string& DiffuseLoc,
		std::string& SpecularLoc, std::string& NormalLoc);
};