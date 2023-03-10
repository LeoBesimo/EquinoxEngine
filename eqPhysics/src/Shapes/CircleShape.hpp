#pragma once

#include "Shape.hpp"

namespace eq
{
	namespace Physics
	{
		class CircleShape :
			public Shape
		{
		private:
			float m_Radius;

		public:
			CircleShape(Math::Vector2 position, float angle, float radius, Material material);

			void update(float delta);
			void update(float delta, int timeSteps);

		public:
			float getRadius() { return this->m_Radius; }
			void setRadius(float radius) { this->m_Radius = radius; }

		private:
			void applyGravity();
			void applyGravity(int timeSteps);

			void calculateBoundingBox();
			void calculateUnits();
		};
	}
}