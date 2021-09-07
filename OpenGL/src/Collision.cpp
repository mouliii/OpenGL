#include "Collision.h"

bool Collision::AABB(const Rect& r1, const Rect& r2)
{
	// pre calc ?
	const float left =   (r1.pos.x - r1.halfSize.x) - (r2.pos.x + r2.halfSize.x);
	const float top =    (r2.pos.y - r2.halfSize.y) - (r1.pos.y + r1.halfSize.y);
	const float right =  (r1.pos.x + r1.halfSize.x) - (r2.pos.x - r2.halfSize.x);
	const float bottom = (r2.pos.y + r2.halfSize.y) - (r1.pos.y - r1.halfSize.y);

	//std::cout << "left: " << left << " right: " << right << " top: " << top << " bot: " << bottom << "\n";

	if (left <= 0.0f && right >= 0.0f && top <= 0.0f && bottom >= 0.0f)
		return true;

	return false;
}

bool Collision::RayVsRect(const Vec2f& rayOrigin, const Vec2f& rayDir, const Rect& rect, Vec2f* contactPoint, Vec2f* contactNormal, float* contactTime)
{ 
	Vec2f rBotLeft = {rect.pos.x - rect.halfSize.x, rect.pos.y - rect.halfSize.y};
	Vec2f rTopRight = { rect.pos.x + rect.halfSize.x, rect.pos.y + rect.halfSize.y };

	Vec2f invDir = { 1.f / rayDir.x, 1.f / rayDir.y };

	Vec2f t_near = (rBotLeft - rayOrigin) * invDir;
	Vec2f t_far = (rTopRight - rayOrigin) * invDir;

	if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
	if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

	if (t_near.x > t_far.x) { std::swap(t_near.x, t_far.x); }
	if (t_near.y > t_far.y) { std::swap(t_near.y, t_far.y); }

	if (t_near.x > t_far.y || t_near.y > t_far.x) { return false; }

	*contactTime = std::max(t_near.x, t_near.y);
	float t_hit_far = std::min(t_far.x, t_far.y);
	
	if (t_hit_far < 0.0f) { return false; } 

	*contactPoint = rayOrigin + *contactTime * rayDir;

	if (t_near.x > t_near.y)
	{
		if (invDir.x < 0.0f)
		{ *contactNormal = Vec2f(1.0f, 0.0f); }
		else
		{ *contactNormal = Vec2f(-1.0f, 0.0f); }
	}
	else if (t_near.x < t_near.y)
	{
		if (invDir.y < 0.0f)
		{ *contactNormal = Vec2f(0.0f, 1.0f); }
		else 
		{ *contactNormal = Vec2f(0.0f, -1.0f); }
	}
	else
	{
		// 0,-1 tai jtn
		*contactNormal = {0.f, 0.f};
	}
	return true;
}

bool Collision::DynamicRectVsRect(const Rect& dynamic, const Vec2f& velocity, const Rect& _static, Vec2f* contactPoint, Vec2f* contactNormal, float* contactTime, const float dt)
{
	// ei toimi jos tekee tän v
	//if (velocity.x == 0.0f || velocity.y == 0.0f)
	//{
	//	return false;
	//}
	Rect rectExpanded(Vec2f(_static.pos), Vec2f(_static.halfSize + dynamic.halfSize), dynamic.color);

	if (RayVsRect(dynamic.pos, velocity * dt, rectExpanded, contactPoint, contactNormal, contactTime))
	{
		return *contactTime >= 0.0f && *contactTime <= 1.0f;
	}
	return false;
}

bool Collision::SweptAABB( Rect& dynamic, Vec2f& vel, Rect& staticRect, const float dt)
{
	Vec2f cn = {0.f,0.f};
	Vec2f cp = { 0.f,0.f };
	float contactTime = 0.0f;
	if (DynamicRectVsRect(dynamic, vel, staticRect, &cp, &cn, &contactTime, dt))
	{
		dynamic.pos = cp;
		float dotProduct = (vel.x * cn.y + vel.y * cn.x) * (1.f - contactTime);
		vel.x = dotProduct * cn.y;
		vel.y = dotProduct * cn.x;
		return true;
	}
	return false;
}

void Collision::CheckAABBCollisions(Rect& dynamic, Vec2f& vel, const std::vector<Rect>& rects, const float dt)
{
	Vec2f cn = { 0.f,0.f };
	Vec2f cp = { 0.f,0.f };
	float contactTime = 0.0f;
	// first pass - check collisions
	std::vector<std::pair<size_t, float>> z;
	for (size_t i = 0; i < rects.size(); i++)
	{
		// calc collision distance
		if (Collision::DynamicRectVsRect(dynamic, vel, rects[i], &cp, &cn, &contactTime, dt))
		{
			z.push_back({ i,contactTime });
		}
	}
	//sort col dist
	std::sort(z.begin(), z.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b) { return a.second < b.second; });

	// second pass - resolve
	for (auto& j : z)
	{
		if (Collision::DynamicRectVsRect(dynamic, vel, rects[j.first], &cp, &cn, &contactTime, dt))
		{
			dynamic.pos = cp;
			float dotProduct = (vel.x * cn.y + vel.y * cn.x) * (1.f - contactTime);
			vel.x = dotProduct * cn.y;
			vel.y = dotProduct * cn.x;
		}
	}
}
