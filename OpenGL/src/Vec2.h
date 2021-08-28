#pragma once

#include <cmath>
#include <iostream>

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
	friend std::ostream& operator<<(std::ostream& stream, const Vec2& vec)
	{
		stream << "( " << vec.x << ", " << vec.y << " )";
		return stream;
	}
	Vec2 operator+(const Vec2& rhs) const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}
	friend Vec2 operator+(const Vec2& lhs, const T rhs)
	{
		return Vec2(lhs.x + rhs, lhs.y + rhs);
	}
	Vec2& operator+=(const Vec2& rhs)
	{
		return *this = *this + rhs;
	}
	Vec2 operator-(const Vec2& rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}
	friend Vec2 operator-(const Vec2& lhs, const T rhs)
	{
		return Vec2(lhs.x - rhs, lhs.y - rhs);
	}
	Vec2& operator-=(const Vec2& rhs)
	{
		return *this = *this - rhs; 
	}
	friend Vec2 operator*(const T lhs, const Vec2& rhs)
	{
		// Vec2 on friend, että se voi olla toinen argumentti Vec2 classissa ???? 
		return rhs * lhs;
	}
	Vec2 operator*(const T rhs) const
	{
		return Vec2(x * rhs, y * rhs);
	}
	Vec2 operator*(const Vec2& rhs) const
	{
		return Vec2(x * rhs.x, y * rhs.y);
	}
	Vec2& operator*=(const Vec2& rhs )
	{
		return *this = *this * rhs;
	}
	Vec2& operator*=(const T rhs)
	{
		return *this = *this * rhs;
	}
	////////////////////////////////////////////////// TODO JAKO 0 //////////////////////////////////////////////////////////////
	Vec2 operator/(const T rhs) const
	{
		return Vec2(x / rhs, y / rhs);
	}
	Vec2& operator/=(const T rhs)
	{
		return *this = *this / rhs;
	}
	Vec2 operator/(const Vec2& rhs) const
	{
		return Vec2(x / rhs.x, y / rhs.y);
	}
	Vec2& operator/=(const Vec2& rhs)
	{
		return *this = *this * rhs;
	}
	Vec2& Inverse()
	{
		return *this = Vec2(1.0f / x, 1.0f / y);
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
	Vec2 Rotate(float theta)
	{
		return *this = GetRotated(theta);
	}
	Vec2 GetRotated(float theta)
	{
		const auto cos = std::cos(theta);
		const auto sin = std::sin(theta);
		return Vec2(cos * x - sin * y, sin * x + cos * y);
	}
public:
	T x;
	T y;
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;