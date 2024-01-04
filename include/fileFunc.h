#pragma once

#include "nodeComponents.h"

enum LinkType {
	LinkType_Normal,
	LinkType_True,
	LinkType_False
};

struct Link {
	void* first;
	void* second;
	LinkType type;
};

std::vector<std::string> GetSchemeFileNames();
bool SaveSchemeToFile(NodeArrays& nodes, std::string path);
bool LoadSchemeFromFile(NodeArrays& nodes, std::string path);