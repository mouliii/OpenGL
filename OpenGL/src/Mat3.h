#pragma once

#include <cmath>
#include "Vec3.h"

template <typename T>
class Mat3
{
public:
	Mat3() = default;
	Vec3<T> operator*(const Vec3<T>& v) const
	{
		Vec3<T> out;
		out.x = cells[0][0] * v.x + cells[0][1] * v.y + cells[0][2] * v.z;
		out.y = cells[1][0] * v.x + cells[1][1] * v.y + cells[1][2] * v.z;
		out.z = cells[2][0] * v.x + cells[2][1] * v.y + cells[2][2] * v.z;

		return out;
	}
	Mat3<T> operator*(const Mat3& rhs) const
	{
		Mat3 out;
		for (int row_left = 0; row_left < 3; row_left++)
		{
			for (int col_right = 0; col_right < 3; col_right++)
			{
				// indeksi
				out.cells[row_left][col_right] = (T)0;
				// kertominen
				for (size_t i = 0; i < 3; i++)
				{
					out.cells[row_left][col_right] += cells[row_left][i] * rhs.cells[i][col_right];
				}
			}
		}
		return out;
	}
	Mat3<T>& operator*=(const Mat3& rhs)
	{
		return *this = *this * rhs;
	}
	static Mat3<T> Scale(T factor)
	{
		return{ factor, (T)0,   (T)0,
				(T)0,   factor, (T)0, 
				(T)0,   (T)0,   (T)1 };
	}
	static Mat3<T> Identity()
	{
		return Scale((T)1);
	}
	static Mat3<T> FlipY()
	{
		return{ (T)1, (T)0, (T)0,
				(T)0, (T)-1,(T)0, 
				(T)0, (T)0, (T)1 };
	}
	static Mat3<T> Rotate(float theta)
	{
		const auto cos = std::cos(theta);
		const auto sin = std::sin(theta);
		return{ cos, -sin,  (T)0,
				sin,  cos,  (T)0,
				(T)0, (T)0, (T)1 }; // 1!
	}
	static Mat3<T> Translate(T x, T y)
	{
		return{ (T)0, (T)0, (T)0,
				(T)0, (T)-1,(T)0,
				(T)0, (T)0, (T)1 };
	}
public:
	//  x   -   y
	// rivi - sarake
	T cells[3][3];
};

typedef Mat3<float> Mat3f;
typedef Mat3<int> Mat3i;