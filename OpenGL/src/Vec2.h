#pragma once

#include <cmath>

template <typename T>
class Vec2
{
public:
	Vec2() = default;
	Vec2(T x_in, T y_in)
		:
		x(x_in),
		y(y_in)
	{}
	template <typename S>
	Vec2(const Vec2<S>& src)
		:
		x((T)src.x),
		y((T)src.y)
	{}
	Vec2 operator+(const Vec2& rhs) const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}
	Vec2& operator+=(const Vec2& rhs)
	{
		return *this = *this + rhs;
	}
	Vec2 operator-(const Vec2& rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}
	Vec2& operator-=(const Vec2& rhs)
	{
		return *this = *this - rhs; 
	}
	Vec2 operator*(T rhs) const
	{
		return Vec2(x * rhs, y * rhs);
	}
	Vec2& operator*=(T rhs)
	{
		return *this = *this * rhs;
	}
	T Length() const
	{
		return (T)std::sqrt(x * x + y * y);
	}
	// muuttaa kutsuvaa objektia
	Vec2& Normalize()
	{
		return *this = GetNormalized();
	}
	// ei muuta objektia returnaa arvon
	Vec2 GetNormalized() const
	{
		const T len = Length();
		if (len != (T)0)
		{
			// invertoitu pituus -- chili tut.
			return *this * ((T)1 / len);
		}
		return *this;
	}
public:
	T x;
	T y;
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;