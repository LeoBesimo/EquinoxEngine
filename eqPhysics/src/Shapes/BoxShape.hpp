#pragma once

#include "Shape.hpp"

namespace eq
{
	namespace Physics
	{
		class BoxShape :
			public Shape
		{
		private:
			Math::Vector2 m_Original[4];
			std::vector<Math::Vector2> m_Transformed;

		public:
			BoxShape(Math::Vector2 position, float angle, Material material, Math::Matrix2x2 scale);

			void update(float delta);

		public:
			std::vector<Math::Vector2> getCorners() { return m_Transformed; };

			void move(Math::Vector2 distance);

		private:
			void applyGravity();
			void transformPoints();
			void calculateUnits();
		};
	}
}