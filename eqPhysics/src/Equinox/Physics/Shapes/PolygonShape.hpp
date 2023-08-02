#pragma once

#include "Shape.hpp"
#include "CollisionUtilities.hpp"
#include "CollisionDetectionAlgorithms.hpp"

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
			void update(float delta, int timeSteps);

			std::vector<Math::Vector2> getCorners() { return this->m_Transformed; }

			void move(Math::Vector2 distance);

			Manifold detectCollision(Shape* other) override;
			Manifold collideLine(Shape* other) override;
			Manifold collideCircle(Shape* other) override;
			Manifold collidePolygon(Shape* other) override;
			Manifold collideBox(Shape* other) override;

		private:
			void applyGravity();
			void applyGravity(int timeSteps);

			void calculateBoundingBox();
			void transformPoints();
			void calculateUnits();
		};
	}
}