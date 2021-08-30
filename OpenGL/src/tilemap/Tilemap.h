#pragma once
#include "json.hpp"
#include <vector>
#include "Rect.h"

class Tilemap
{
public:
	Tilemap(const std::string& tiledFilePath);
private:
	std::vector<std::vector<Rect>> rects;
};