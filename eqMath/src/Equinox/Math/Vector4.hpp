#pragma once

#include <cmath>

namespace eq
{
	namespace Math
	{
		struct Vector4
		{
			float w = 0;
			float x = 0;
			float y = 0;
			float z = 0;

			Vector4(float w = 0, float x = 0, float y = 0, float z = 0) :
				w(w), x(x), y(y), z(z)
			{}


			Vector4 operator-(void) const
			{
				return Vector4(-w, -x, -y, -z);
			}

			Vector4 operator-(const Vector4& a) const
			{
				return Vector4(w - a.w, x - a.x, y - a.y, z - a.z);
			}

			Vector4 operator-(float a) const
			{
				return Vector4(w - a, x - a, y - a, z - a);
			}

			Vector4 operator+(void) const
			{
				return Vector4(+w, +x, +y, +z);
			}

			Vector4 operator+(const Vector4& a) const
			{
				return Vector4(w + a.w, x + a.x, y + a.y, z + a.z);
			}

			Vector4 operator+(float a) const
			{
				return Vector4(w + a, x + a, y + a, z + a);
			}

			Vector4 operator*(const Vector4& a) const
			{
				return Vector4(w * a.w, x * a.x, y * a.y, z * a.z);
			}

			Vector4 operator*(float a) const
			{
				return Vector4(w * a, x * a, y * a, z * a);
			}

			Vector4 operator/(const Vector4& a) const
			{
				return Vector4(w / a.w, x / a.x, y / a.y, z / a.z);
			}

			Vector4 operator/(float a) const
			{
				return Vector4(w / a, x / a, y / a, z / a);
			}

			void operator-=(const Vector4& a)
			{
				w -= a.w;
				x -= a.x;
				y -= a.y;
				z -= a.z;
			}

			void operator+=(const Vector4& a)
			{
				w += a.w;
				x += a.x;
				y += a.y;
				z += a.z;
			}

			void operator*=(const float a)
			{
				w *= a;
				x *= a;
				y *= a;
				z *= a;
			}

			void operator*=(const Vector4& a)
			{
				w *= a.w;
				x *= a.x;
				y *= a.y;
				z *= a.z;
			}

			void operator/=(const float a)
			{
				w /= a;
				x /= a;
				y /= a;
				z /= a;
			}

			void operator/=(const Vector4& a)
			{
				w /= a.w;
				x /= a.x;
				y /= a.y;
				z /= a.z;
			}

			float lenSqr()
			{
				return w * w + x * x + y * y + z * z;
			}

			float len()
			{
				return std::sqrt(w * w + x * x + y * y + z * z);
			}

			Vector4 normalize()
			{
				float l = len();

				if (l == 0) return Vector4(0, 0, 0, 0);

				float invLen = 1.0f / l;

				w *= invLen;
				x *= invLen;
				y *= invLen;
				z *= invLen;

				return *this;
			}
		};
	}
}