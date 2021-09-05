#pragma once

#include <cmath>
#include "Vec2.h"

template <typename T>
class Mat4
{
public:
	Mat4() = default;
	// TODO vec3?/
	Vec2<T> operator*(const Vec2<T>& v) const
	{
		Vec2<T> out;
		out.x = cells[0][0] * v.x + cells[0][1] * v.y;
		out.y = cells[1][0] * v.x + cells[1][1] * v.y;
		return out;
	}
	Mat4 operator*(const Mat4& rhs) const
	{
		Mat4 out;
		for (int row_left = 0; row_left < 4; row_left++)
		{
			for (int col_right = 0; col_right < 4; col_right++)
			{
				// indeksi
				out.cells[row_left][col_right] = (T)0;
				// kertominen
				for (size_t i = 0; i < 4; i++)
				{
					out.cells[row_left][col_right] += cells[row_left][i] * rhs.cells[i][col_right];
				}
			}
		}
		return out;
	}
	Mat4& operator*=(const Mat4& rhs)
	{
		return *this = *this * rhs;
	}
	static Mat4 Ortho(T left, T right, T bottom, T top, T zNear, T zFar)
	{
		return{ 2/(right-left), (T)0,			(T)0,	 -((right+left) / (right-left)),
				(T)0,			2/(top-bottom), (T)0,	 -((top + bottom) / (top - bottom)),
				(T)0,			(T)0,					 -2/(zFar - zNear),-((zFar + zNear) / (zFar - zNear)),
				(T)0,			(T)0,			(T)0,	 1 };
	}
	static Mat4 Scale(T factor)
	{
		return{ factor, (T)0, (T)0, (T)0,
				(T)0, factor, (T)0, (T)0,
				(T)0,   (T)0, factor, (T)0,
				(T)0,   (T)0,  (T)0, factor};
	}
	// TODO
	static Mat4 Rotate(float theta)
	{
		const auto cos = std::cos(theta);
		const auto sin = std::sin(theta);
		return{ cos, -sin,
				sin, cos };
	}
public:
	//  x   -   y
	// rivi - sarake
	T cells[4][4];
};

typedef Mat4<float> Mat4f;
typedef Mat4<int> Mat4i;