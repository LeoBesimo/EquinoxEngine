#pragma once

#include <eqMath.hpp>
#include "Shapes/eqShapes.hpp"

namespace eq
{
	namespace Physics
	{
		class CollisionDetector
		{
		private:
			Math::Vector2 getContactBoxBox(BoxShape* bodyA, BoxShape* bodyB);
			Math::Vector2 getContactPolygonPolygon(PolygonShape* bodyA, PolygonShape* bodyB);
			Math::Vector2 getContactBoxPolygon(BoxShape* bodyA, PolygonShape* bodyB);
			Math::Vector2 getContactCircleBox(CircleShape* bodyA, BoxShape* bodyB);
			Math::Vector2 getcontactCirclePolygon(CircleShape* bodyA, PolygonShape* bodyB);

			std::vector<Math::Vector2> getNormals(std::vector<Math::Vector2> corners);

			Math::Vector2 getMinMax(std::vector<Math::Vector2> points, Math::Vector2 normal);
			Math::Vector2 getMinMaxCircle(Math::Vector2 center, float radius, Math::Vector2 normal);

			Math::Vector2 getClosestPoint(Math::Vector2 center, std::vector<Math::Vector2> points);

			Manifold CircleCircleCollision(CircleShape* bodyA, CircleShape* bodyB);
			Manifold PolygonPolygonCollision(PolygonShape* bodyA, PolygonShape* bodyB);
			Manifold BoxBoxCollision(BoxShape* bodyA, BoxShape* bodyB);

			Manifold CircleBoxCollison(CircleShape* bodyA, BoxShape* bodyB);
			Manifold CirclePolygonCollison(CircleShape* bodyA, PolygonShape* bodyB);
			Manifold BoxPolygonCollision(BoxShape* bodyA, PolygonShape* bodyB);

		public:
			CollisionDetector();
			bool boundingBoxCollision(Shape* bodyA, Shape* bodyB);
			Manifold detectCollision(Shape* bodyA, Shape* bodyB);

		};
	}
}