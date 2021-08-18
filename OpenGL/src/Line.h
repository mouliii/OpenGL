#pragma once
#include "Vec2.h"
#include "OrthoCamera.h"

class Line
{
public:
	Line(Vec2f start, Vec2f end)
	{
	}
	void Draw(OrthoCamera* cam)
	{

	}
private:
	Vec2f positions[2];
	int indices[2] = {0,1};
};