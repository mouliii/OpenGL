#pragma once
#include "Rect.h"


class Collision
{
public:
	// minkowski aabb
	static bool AABB(const Rect& r1, const Rect& r2);
	static bool RayVsRect(const Vec2f& rayOrigin, const Vec2f& rayDir, const Rect& rect, Vec2f* contactPoint, Vec2f* contactNormal, float* contactTime);
	static bool DynamicRectVsRect(const Rect& dynamic, const Vec2f& velocity, const Rect& _static, Vec2f* contactPoint, Vec2f* contactNormal, float* contactTime, const float dt);
	static bool SweptAABB(Rect& dynamic, Vec2f& vel, const Rect& staticRect, const float dt);
	static void ResolveAABBCollisions(Rect& dynamic, Vec2f& vel, const std::vector<Rect>& rects, const float dt);
private:

};
