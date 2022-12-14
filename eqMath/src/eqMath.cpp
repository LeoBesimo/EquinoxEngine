#include "eqMath.hpp"

namespace eq
{
	namespace Math
	{
		float pythagoreanSolve(float x, float y)
		{
			return x * x + y * y;
		}

		float pythagoreanSolve(float x, float y, float z)
		{
			return x * x + y * y + z * z;
		}

		float pythagoreanSolve(float w, float x, float y, float z)
		{
			return w * w + x * x + y * y + z * z;
		}

		float pythagoreanSolve(Vector2 a, Vector2 b)
		{
			return pythagoreanSolve(a.x - b.x, a.y - b.y);
		}

		float pythagoreanSolve(Vector3 a, Vector3 b)
		{
			return pythagoreanSolve(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		float pythagoreanSolve(Vector4 a, Vector4 b)
		{
			return pythagoreanSolve(a.w - b.w, a.x - b.x, a.y - b.y, a.z - b.z);
		}

		float fastInvSquareRoot(float n)
		{
			const float threehalfs = 1.5F;
			float y = n;

			long i = *(long*)&y;

			i = 0x5f3759df - (i >> 1);
			y = *(float*)&i;

			y = y * (threehalfs - ((n * 0.5F) * y * y));
			y = y * (threehalfs - ((n * 0.5F) * y * y));

			return y;
		}

		float dot(Vector2 a, Vector2 b)
		{
			return a.x * b.x + a.y * b.y;
		}

		float dot(Vector3 a, Vector3 b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		float dot(Vector4 a, Vector4 b)
		{
			return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
		}

		float cross(Vector2 a, Vector2 b)
		{
			return a.x * b.y - a.y * b.x;
		}

		Vector3 cross(Vector3 a, Vector3 b)
		{
			return Vector3(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			);
		}

		float square(float n)
		{
			return n * n;
		}
		bool nearlyEqual(float a, float b)
		{
			return abs(a - b) < THRESHOLD;
		}
		bool nearlyEqual(Vector2 a, Vector2 b)
		{
			return nearlyEqual(a.x, b.x) && nearlyEqual(a.y, b.y);
		}
		bool nearlyEqual(Vector3 a, Vector3 b)
		{
			return nearlyEqual(a.x, b.x) && nearlyEqual(a.y, b.y) && nearlyEqual(a.z, b.z);
		}
		bool nearlyEqual(Vector4 a, Vector4 b)
		{
			return nearlyEqual(a.w, b.w) && nearlyEqual(a.x, b.x) && nearlyEqual(a.y, b.y) && nearlyEqual(a.z, b.z);
		}

		float distPointToLine(Math::Vector2 point, Math::Vector2 a, Math::Vector2 b, Math::Vector2* closest)
		{
			Math::Vector2 ab = b - a;
			Math::Vector2 ap = point - a;

			float proj = Math::dot(ap, ab);
			float abLenSqr = ab.lenSqr();
			float d = proj / abLenSqr;

			if (d <= 0)
			{
				closest->x = a.x;
				closest->y = a.y;
			}
			else if (d >= 1.0f)
			{
				closest->x = b.x;
				closest->y = b.y;
			}
			else
			{
				Math::Vector2 c = (a + (ab * d));
				closest->x = c.x;
				closest->y = c.y;
			}

			double dx = point.x - closest->x;
			double dy = point.y - closest->y;

			return dx * dx + dy * dy;
		}
	}
}