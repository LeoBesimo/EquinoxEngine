#pragma once

#include "Vector2.hpp"


namespace eq
{
	namespace Math
	{
		struct Matrix2x2
		{
			Vector2 a, b;

			Matrix2x2(Vector2 a = Vector2(), Vector2 b = Vector2()) :
				a(a), b(b) {}

			Matrix2x2(float a, float b, float c, float d) :
				a(a, c), b(b, d) {}

			Matrix2x2(float angle)
			{
				float c = cos(angle);
				float s = sin(angle);

				a = Vector2(c, s);
				b = Vector2(-s, c);
			}

			Matrix2x2 operator-(const Matrix2x2& m) const
			{
				return Matrix2x2(a - m.a, b - m.b);
			}

			Matrix2x2 operator+(const Matrix2x2& m) const
			{
				return Matrix2x2(a + m.a, b + m.b);
			}

			Matrix2x2 operator*(const float& s) const
			{
				return Matrix2x2(a * s, b * s);
			}

			Matrix2x2 operator*(const Matrix2x2& m) const
			{
				float p1 = a.x * m.a.x + b.x * m.a.y;
				float p2 = a.x * m.b.x + b.x * m.b.y;
				float p3 = a.y * m.a.x + b.y * m.a.y;
				float p4 = a.y * m.b.x + b.y * m.b.y;
				return Matrix2x2(p1, p2, p3, p4);
			}

			Vector2 operator*(const Vector2& v) const
			{
				return a * v.x + b * v.y;
			}


			/*bool operator==(Matrix2x2 mat)
			{
				return a == mat.a && b == mat.b;
			}

			bool operator!=(Matrix2x2 mat)
			{
				return a != mat.a || b != mat.b;
			}*/

			float determinant()
			{
				return a.cross(b);
			}

			Matrix2x2 inverse()
			{
				float det = determinant();
				if (det == 0) return Matrix2x2();

				float invDet = 1 / det;
				return Matrix2x2(b.y, -a.y, -b.x, a.x) * invDet;
			}

			static Matrix2x2 unit()
			{
				return Matrix2x2(1, 0, 0, 1);
			}

		};
	}
}