#pragma once
#include "Vec2.h"
#include <cmath>

template <typename T>
class Vec3
{
public:
	Vec3() = default;
	Vec3(T x_in, T y_in, T z_in)
		:
		x(x_in),
		y(y_in),
		z(z_in)
	{}
	template <typename S>
	Vec3(const Vec3<S>& src)
		:
		x((T)src.x),
		y((T)src.y),
		z((T)src.z)
	{}
	template <typename S>
	Vec3(const Vec2<S> src, T z)
		:
		x(src.x), y(src.y),
		z(z)
	{}
	Vec3 operator+(const Vec3& rhs) const
	{
		return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	Vec3& operator+=(const Vec3& rhs)
	{
		return *this = *this + rhs;
	}
	Vec3 operator-(const Vec3& rhs) const
	{
		return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	Vec3& operator-=(const Vec3& rhs)
	{
		return *this = *this - rhs;
	}
	Vec3 operator*(T rhs) const
	{
		return Vec3(x * rhs, y * rhs, z * rhs);
	}
	Vec3& operator*=(T rhs)
	{
		return *this = *this * rhs;
	}
	Vec3 operator/(T rhs) const
	{
		return Vec3(x / rhs, y / rhs, z / rhs);
	}
	Vec3& operator/=(T rhs)
	{
		return *this = *this / rhs;
	}
	T Length() const
	{
		return (T)std::sqrt(x * x + y * y + z * z);
	}
	// muuttaa kutsuvaa objektia
	Vec3& Normalize()
	{
		return *this = GetNormalized();
	}
	// ei muuta objektia returnaa arvon
	Vec3 GetNormalized() const
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
	T z = (T)1;
};

typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;