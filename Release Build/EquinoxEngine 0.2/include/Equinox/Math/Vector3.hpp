#pragma once

#include <cmath>

namespace eq
{
	namespace Math
	{
		struct Vector3
		{
			float x = 0;
			float y = 0;
			float z = 0;

			Vector3(float x = 0, float y = 0, float z = 0) :
				x(x), y(y), z(z)
			{}

			Vector3 operator-(void) const
			{
				return Vector3(-x, -y, -z);
			}

			Vector3 operator-(const Vector3& a) const
			{
				return Vector3(x - a.x, y - a.y, z - a.z);
			}

			Vector3 operator-(float a) const
			{
				return Vector3(x - a, y - a, z - a);
			}

			Vector3 operator+(void) const
			{
				return Vector3(+x, +y, +z);
			}

			Vector3 operator+(const Vector3& a) const
			{
				return Vector3(x + a.x, y + a.y, z + a.z);
			}

			Vector3 operator+(float a) const
			{
				return Vector3(x + a, y + a, z + a);
			}

			Vector3 operator*(const Vector3& a) const
			{
				return Vector3(x * a.x, y * a.y, z * a.z);
			}

			Vector3 operator*(float a) const
			{
				return Vector3(x * a, y * a, z * a);
			}

			Vector3 operator/(const Vector3& a) const
			{
				return Vector3(x / a.x, y / a.y, z / a.z);
			}

			Vector3 operator/(float a) const
			{
				return Vector3(x / a, y / a, z / a);
			}

			void operator-=(const Vector3& a)
			{
				x -= a.x;
				y -= a.y;
				z -= a.z;
			}

			void operator+=(const Vector3& a)
			{
				x += a.x;
				y += a.y;
				z += a.z;
			}

			void operator*=(const float a)
			{
				x *= a;
				y *= a;
				z *= a;
			}

			void operator*=(const Vector3& a)
			{
				x *= a.x;
				y *= a.y;
				z *= a.z;
			}

			void operator/=(const float a)
			{
				x /= a;
				y /= a;
				z /= a;
			}

			void operator/=(const Vector3& a)
			{
				x /= a.x;
				y /= a.y;
				z /= a.z;
			}

			float lenSqr()
			{
				return x * x + y * y + z * z;
			}

			float len()
			{
				return std::sqrt(x * x + y * y + z * z);
			}

			Vector3 normalize()
			{
				float l = len();

				if (l == 0) return Vector3(0, 0, 0);

				float invLen = 1.0f / l;

				x *= invLen;
				y *= invLen;
				z *= invLen;

				return *this;
			}
		};
	}
}