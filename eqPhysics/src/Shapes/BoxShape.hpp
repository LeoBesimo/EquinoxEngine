#pragma once

#include "Shape.hpp"
#include "CollisionUtilities.hpp"


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
			void update(float delta, int timeSteps);

		public:
			std::vector<Math::Vector2> getCorners() { return m_Transformed; };

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