#pragma once

#include "Shape.hpp"
#include "CollisionUtilities.hpp"
#include <Equinox/Math/eqMath.hpp>

namespace eq {
	namespace Physics {

		Manifold CircleCircleCollision(Shape* bodyA, Shape* bodyB);
		Manifold PolygonPolygonCollision(Shape* bodyA, Shape* bodyB);
		Manifold BoxBoxCollision(Shape* bodyA, Shape* bodyB);

		Manifold CircleBoxCollison(Shape* bodyA, Shape* bodyB);
		Manifold CirclePolygonCollison(Shape* bodyA, Shape* bodyB);
		Manifold BoxPolygonCollision(Shape* bodyA, Shape* bodyB);

		Manifold LineCircleCollision(Shape* bodyA, Shape* bodyB);
		Manifold LinePolygonCollision(Shape* bodyA, Shape* bodyB);
		Manifold LineBoxCollision(Shape* bodyA, Shape* bodyB);
		Manifold LineLineCollision(Shape* bodyA, Shape* bodyB);
	}
}