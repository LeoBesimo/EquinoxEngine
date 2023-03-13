#pragma once

#include "Shape.hpp"

namespace eq
{
	namespace Physics
	{
		class LineShape :
			public Shape
		{
		private:
			Math::Vector2 m_StartPos, m_EndPos;
			float m_Length;

		public:
			LineShape(Math::Vector2 startPos, Math::Vector2 endPos, Material material);
			LineShape(Math::Vector2 pos, float angle, float length, Material material);

			void update(float delta);
			void update(float delta, int timeSteps);

		private:
			void applyGravity();
			void applyGravity(int timeSteps);

			void calculateBoundingBox();
			void calculateUnits();
		};
	}
}