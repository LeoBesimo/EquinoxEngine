#pragma once

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix2x2.hpp"

namespace eq
{
	namespace Math
	{
		const float PI = (float)atan(1) * 4;
		const float QUARTER_PI = (float)(PI / 4.0);
		const float HALF_PI = (float)(PI / 2.0);
		const float TWO_PI = (float)(PI * 2.0);
		const float E = (float)(2.7182818284590452353602874713527);

		const float THRESHOLD = 1.0f;

		extern float pythagoreanSolve(float x, float y);
		extern float pythagoreanSolve(float x, float y, float z);
		extern float pythagoreanSolve(float w, float x, float y, float z);
		extern float pythagoreanSolve(Vector2 a, Vector2 b);
		extern float pythagoreanSolve(Vector3 a, Vector3 b);
		extern float pythagoreanSolve(Vector4 a, Vector4 b);
		extern float fastInvSquareRoot(float n);

		extern float dot(Vector2 a, Vector2 b);
		extern float dot(Vector3 a, Vector3 b);
		extern float dot(Vector4 a, Vector4 b);

		extern float cross(Vector2 a, Vector2 b);
		extern Vector3 cross(Vector3 a, Vector3 b);

		extern float map(float val, float oldMin, float oldMax, float newMin, float newMax);

		extern float square(float n);

		extern bool nearlyEqual(float a, float b);
		extern bool nearlyEqual(Vector2 a, Vector2 b);
		extern bool nearlyEqual(Vector3 a, Vector3 b);
		extern bool nearlyEqual(Vector4 a, Vector4 b);

		extern float distPointToLine(Vector2 point, Vector2 a, Vector2 b, Vector2* closest);

	}
}