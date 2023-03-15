#include "CollisionUtilities.hpp"
#include "BoxShape.hpp"
#include "CircleShape.hpp"
#include "PolygonShape.hpp"
#include "LineShape.hpp"

namespace eq
{

	std::vector<eq::Math::Vector2> eq::Physics::getNormals(std::vector<Math::Vector2> corners)
	{
		std::vector<Math::Vector2> normals(corners.size());

		for (unsigned int i = 0; i < corners.size(); i++)
		{
			unsigned int j = (i + 1) % corners.size();
			Math::Vector2 edge = corners[j] - corners[i];
			normals[i] = (Math::Vector2(-edge.y, edge.x));
		}
		return normals;
	}

	Math::Vector2 eq::Physics::getMinMax(std::vector<Math::Vector2> corners, Math::Vector2 normal)
	{
		float minProj = Math::dot(corners[0], normal);
		float maxProj = Math::dot(corners[0], normal);

		for (unsigned int i = 1; i < corners.size(); i++)
		{
			float currentProj = Math::dot(corners[i], normal);
			if (currentProj < minProj)
			{
				minProj = currentProj;
			}

			if (currentProj > maxProj)
			{
				maxProj = currentProj;
			}
		}
		return Math::Vector2(minProj, maxProj);
	}

	Math::Vector2 eq::Physics::getMinMaxCircle(Math::Vector2 center, float radius, Math::Vector2 normal)
	{
		Math::Vector2 direction = normal;
		direction.normalize();
		direction *= radius;

		Math::Vector2 p1 = center + direction;
		Math::Vector2 p2 = center - direction;

		float min = Math::dot(normal, p1);
		float max = Math::dot(normal, p2);

		if (min > max)
		{
			float t = min;
			min = max;
			max = t;
		}

		return Math::Vector2(min, max);
	}

	Math::Vector2 eq::Physics::getClosestPoint(Math::Vector2 center, std::vector<Math::Vector2> points)
	{
		float minDistance = FLT_MAX;
		Math::Vector2 closest;
		Math::Vector2 cp;
		for (unsigned int i = 0; i < points.size(); i++)
		{
			float dist = Math::distPointToLine(center, points[i], points[(i + 1) % points.size()], &cp);
			if (dist < minDistance)
			{
				minDistance = dist;
				closest = cp;
			}
		}

		return closest;
	}

	Math::Vector2 eq::Physics::getContactBoxBox(Shape* bodyA, Shape* bodyB)
	{
		return Math::Vector2();
	}

	Math::Vector2 eq::Physics::getContactPolygonPolygon(Shape* bodyA, Shape* bodyB)
	{
		return Math::Vector2();
	}

	Math::Vector2 eq::Physics::getContactBoxPolygon(Shape* bodyA, Shape* bodyB)
	{
		return Math::Vector2();
	}

	Math::Vector2 eq::Physics::getContactCircleBox(Shape* bodyA, Shape* bodyB)
	{
		return Math::Vector2();
	}

	Math::Vector2 eq::Physics::getContactCirclePolygon(Shape* bodyA, Shape* bodyB)
	{
		return Math::Vector2();
	}

	Math::Vector2 eq::Physics::getContactLineCircle(Shape* bodyA, Shape* bodyB)
	{
		return Math::Vector2();
	}

	Math::Vector2 eq::Physics::getContactLinePolygon(Shape* bodyA, Shape* bodyB)
	{
		return Math::Vector2();
	}

	Math::Vector2 eq::Physics::getContactLineBox(Shape* bodyA, Shape* bodyB)
	{
		return Math::Vector2();
	}

}