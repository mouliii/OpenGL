#pragma once
#include "Rect.h"


class Collision
{
public:
	// minkowski aabb
	static bool AABB(const Rect& r1, const Rect& r2);
	static bool RayVsRect(const Vec2f& rayOrigin, const Vec2f& rayDir, const Rect& rect, Vec2f* contactPoint, Vec2f* contactNormal, float* t_hit_near);
	static bool DynamicRectVsRect(const Rect& dynamic, const Vec2f& velocity, const Rect& _static, Vec2f* contactPoint, Vec2f* contactNormal, float* t_hit_near, const float dt);
private:

};
