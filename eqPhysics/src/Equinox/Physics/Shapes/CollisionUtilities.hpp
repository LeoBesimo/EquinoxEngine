#pragma once

#include <vector>
#include <Equinox/Math/eqMath.hpp>
#include "Shape.hpp"

namespace eq
{
	namespace Physics
	{
		Math::Vector2 getContactBoxBox(Shape* bodyA, Shape* bodyB);
		Math::Vector2 getContactPolygonPolygon(Shape* bodyA, Shape* bodyB);
		Math::Vector2 getContactBoxPolygon(Shape* bodyA, Shape* bodyB);
		Math::Vector2 getContactCircleBox(Shape* bodyA, Shape* bodyB);
		Math::Vector2 getContactCirclePolygon(Shape* bodyA, Shape* bodyB);

		Math::Vector2 getContactLineCircle(Shape* bodyA, Shape* bodyB);
		Math::Vector2 getContactLinePolygon(Shape* bodyA, Shape* bodyB);
		Math::Vector2 getContactLineBox(Shape* bodyA, Shape* bodyB);

		std::vector<Math::Vector2> getNormals(std::vector<Math::Vector2> corners);

		Math::Vector2 getMinMax(std::vector<Math::Vector2> corners, Math::Vector2 normal);
		Math::Vector2 getMinMaxCircle(Math::Vector2 center, float radius, Math::Vector2 normal);

		Math::Vector2 getClosestPoint(Math::Vector2 center, std::vector<Math::Vector2> points);

		bool boundingBoxCollision(Shape* bodyA, Shape* bodyB);

	}
}