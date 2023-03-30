#pragma once

#include "Shape.hpp"
#include "CollisionUtilities.hpp"


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

			Math::Vector2 getStartPosition() { return m_StartPos; }
			Math::Vector2 getEndPosition() { return m_EndPos; }
			float getLength() { return m_Length; }

			void update(float delta);
			void update(float delta, int timeSteps);

			Manifold detectCollision(Shape* other);
			Manifold collideLine(Shape* other);
			Manifold collideCircle(Shape* other);
			Manifold collidePolygon(Shape* other);
			Manifold collideBox(Shape* other);

		private:
			void applyGravity();
			void applyGravity(int timeSteps);

			void calculateBoundingBox();
			void calculateUnits();
		};
	}
}