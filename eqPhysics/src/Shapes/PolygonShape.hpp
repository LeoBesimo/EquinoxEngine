#pragma once

#include "Shape.hpp"

namespace eq
{
	namespace Physics
	{
		class PolygonShape :
			public Shape
		{
		private:
			std::vector<Math::Vector2> m_Original;
			std::vector<Math::Vector2> m_Transformed;
			float m_Sides;

		public:
			PolygonShape(Math::Vector2 position, float angle, float sides, Material material, Math::Matrix2x2 scale);

			void update(float delta);

			std::vector<Math::Vector2> getCorners() { return this->m_Transformed; }

			void move(Math::Vector2 distance);

		private:
			void applyGravity();
			void transformPoints();
			void calculateUnits();
		};
	}
}