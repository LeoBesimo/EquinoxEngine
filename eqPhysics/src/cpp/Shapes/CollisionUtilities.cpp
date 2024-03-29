#include "../../Equinox/Physics/Shapes/CollisionUtilities.hpp"
#include "../../Equinox/Physics/Shapes/BoxShape.hpp"
#include "../../Equinox/Physics/Shapes/CircleShape.hpp"
#include "../../Equinox/Physics/Shapes/PolygonShape.hpp"
#include "../../Equinox/Physics/Shapes/LineShape.hpp"

namespace eq
{
	class Physics::BoxShape;
	class Physics::CircleShape;
	class Physics::PolygonShape;
	class Physics::LineShape;

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

	bool Physics::boundingBoxCollision(Shape* bodyA, Shape* bodyB)
	{
		Math::Vector2 bodyAMin = bodyA->getPosition() + bodyA->getBoundingMin();
		Math::Vector2 bodyAMax = bodyA->getPosition() + bodyA->getBoundingMax();
		Math::Vector2 bodyBMin = bodyB->getPosition() + bodyB->getBoundingMin();
		Math::Vector2 bodyBMax = bodyB->getPosition() + bodyB->getBoundingMax();
		return !(bodyAMax.x < bodyBMin.x || bodyAMin.x > bodyBMax.x || bodyAMax.y < bodyBMin.y || bodyAMin.y > bodyBMax.y);
	}

	Math::Vector2 eq::Physics::getContactBoxBox(Shape* a, Shape* b)
	{
		BoxShape* bodyA = static_cast<BoxShape*>(a);
		BoxShape* bodyB = static_cast<BoxShape*>(b);

		std::vector<Math::Vector2> pointsA = bodyA->getCorners();
		std::vector<Math::Vector2> pointsB = bodyB->getCorners();
		unsigned int contactCount = 0;

		Math::Vector2 contact1;
		Math::Vector2 contact2;

		float minDistSqr = FLT_MAX;

		Math::Vector2 closest;

		for (unsigned int i = 0; i < 4; i++)
		{

			Math::Vector2 p = pointsA[i];

			for (unsigned int j = 0; j < 4; j++)
			{
				Math::Vector2 a = pointsB[j];
				Math::Vector2 b = pointsB[(j + 1) % pointsB.size()];

				float distSqr = Math::distPointToLine(p, a, b, &closest);

				if (Math::nearlyEqual(distSqr, minDistSqr))
				{
					if (!Math::nearlyEqual(closest, contact1))
					{
						contactCount = 2;
						contact2 = closest;
					}
				}
				else if (distSqr < minDistSqr)
				{
					minDistSqr = distSqr;
					contactCount = 1;
					contact1 = closest;
				}
			}

		}

		for (unsigned int i = 0; i < 4; i++)
		{

			Math::Vector2 p = pointsB[i];

			for (unsigned int j = 0; j < 4; j++)
			{
				Math::Vector2 a = pointsA[j];
				Math::Vector2 b = pointsA[(j + 1) % 4];

				float distSqr = Math::distPointToLine(p, a, b, &closest);

				if (Math::nearlyEqual(distSqr, minDistSqr))
				{
					if (!nearlyEqual(closest, contact1))
					{
						contactCount = 2;
						contact2 = closest;
					}
				}
				else if (distSqr < minDistSqr)
				{
					minDistSqr = distSqr;
					contactCount = 1;
					contact1 = closest;
				}
			}
		}

		Math::Vector2 contact = contact1;
		if (contactCount == 2) contact = (contact1 + contact2) / 2;

		return contact;
	}

	Math::Vector2 eq::Physics::getContactPolygonPolygon(Shape* a, Shape* b)
	{
		PolygonShape* bodyA = static_cast<PolygonShape*>(a);
		PolygonShape* bodyB = static_cast<PolygonShape*>(b);

		std::vector<Math::Vector2> pointsA = bodyA->getCorners();
		std::vector<Math::Vector2> pointsB = bodyB->getCorners();
		unsigned int contactCount = 0;

		Math::Vector2 contact1;
		Math::Vector2 contact2;

		float minDistSqr = FLT_MAX;

		Math::Vector2 closest;

		for (unsigned int i = 0; i < pointsA.size(); i++)
		{

			Math::Vector2 p = pointsA[i];

			for (unsigned int j = 0; j < pointsB.size(); j++)
			{
				Math::Vector2 a = pointsB[j];
				Math::Vector2 b = pointsB[(j + 1) % pointsB.size()];

				float distSqr = Math::distPointToLine(p, a, b, &closest);

				if (Math::nearlyEqual(distSqr, minDistSqr))
				{
					if (!nearlyEqual(closest, contact1))
					{
						contactCount = 2;
						contact2 = closest;
					}
				}
				else if (distSqr < minDistSqr)
				{
					minDistSqr = distSqr;
					contactCount = 1;
					contact1 = closest;
				}
			}

		}

		for (unsigned int i = 0; i < pointsB.size(); i++)
		{

			Math::Vector2 p = pointsB[i];

			for (unsigned int j = 0; j < pointsA.size(); j++)
			{
				Math::Vector2 a = pointsA[j];
				Math::Vector2 b = pointsA[(j + 1) % pointsA.size()];

				float distSqr = Math::distPointToLine(p, a, b, &closest);

				if (Math::nearlyEqual(distSqr, minDistSqr))
				{
					if (!nearlyEqual(closest, contact1))
					{
						contactCount = 2;
						contact2 = closest;
					}
				}
				else if (distSqr < minDistSqr)
				{
					minDistSqr = distSqr;
					contactCount = 1;
					contact1 = closest;
				}
			}
		}

		Math::Vector2 contact = contact1;
		if (contactCount == 2) contact = (contact1 + contact2) / 2;

		return contact;
	}

	Math::Vector2 eq::Physics::getContactBoxPolygon(Shape* a, Shape* b)
	{
		BoxShape* bodyA = static_cast<BoxShape*>(a);
		PolygonShape* bodyB = static_cast<PolygonShape*>(b);

		std::vector<Math::Vector2> pointsA = bodyA->getCorners();
		std::vector<Math::Vector2> pointsB = bodyB->getCorners();
		unsigned int contactCount = 0;

		Math::Vector2 contact1;
		Math::Vector2 contact2;

		float minDistSqr = FLT_MAX;

		Math::Vector2 closest;

		for (unsigned int i = 0; i < 4; i++)
		{

			Math::Vector2 p = pointsA[i];

			for (unsigned int j = 0; j < pointsB.size(); j++)
			{
				Math::Vector2 a = pointsB[j];
				Math::Vector2 b = pointsB[(j + 1) % pointsB.size()];

				float distSqr = Math::distPointToLine(p, a, b, &closest);

				if (Math::nearlyEqual(distSqr, minDistSqr))
				{
					if (!nearlyEqual(closest, contact1))
					{
						contactCount = 2;
						contact2 = closest;
					}
				}
				else if (distSqr < minDistSqr)
				{
					minDistSqr = distSqr;
					contactCount = 1;
					contact1 = closest;
				}
			}

		}

		for (unsigned int i = 0; i < pointsB.size(); i++)
		{

			Math::Vector2 p = pointsB[i];

			for (unsigned int j = 0; j < 4; j++)
			{
				Math::Vector2 a = pointsA[j];
				Math::Vector2 b = pointsA[(j + 1) % 4];

				float distSqr = Math::distPointToLine(p, a, b, &closest);

				if (Math::nearlyEqual(distSqr, minDistSqr))
				{
					if (!nearlyEqual(closest, contact1))
					{
						contactCount = 2;
						contact2 = closest;
					}
				}
				else if (distSqr < minDistSqr)
				{
					minDistSqr = distSqr;
					contactCount = 1;
					contact1 = closest;
				}
			}
		}

		Math::Vector2 contact = contact1;
		if (contactCount == 2) contact = (contact1 + contact2) / 2;

		return contact;
	}

	Math::Vector2 eq::Physics::getContactCircleBox(Shape* a, Shape* b)
	{
		CircleShape* bodyA = static_cast<CircleShape*>(a);
		BoxShape* bodyB = static_cast<BoxShape*>(b);

		std::vector<Math::Vector2> corners = bodyB->getCorners();
		Math::Vector2 pCenter = bodyB->getPosition();
		Math::Vector2 cCenter = bodyA->getPosition();
		float radius = bodyA->getRadius();
		Math::Vector2 cp;

		float minDistSqr = FLT_MAX;

		for (unsigned int i = 0; i < corners.size(); i++)
		{
			int j = (i + 1) % corners.size();
			Math::Vector2 va = corners[i];
			Math::Vector2 vb = corners[j];

			Math::Vector2 contact;
			float distSqr = Math::distPointToLine(cCenter, va, vb, &contact);
			if (distSqr < minDistSqr)
			{
				cp = contact;
				minDistSqr = distSqr;
			}
		}

		return cp;
	}

	Math::Vector2 eq::Physics::getContactCirclePolygon(Shape* a, Shape* b)
	{
		CircleShape* bodyA = static_cast<CircleShape*>(a);
		PolygonShape* bodyB = static_cast<PolygonShape*>(b);

		std::vector<Math::Vector2> corners = bodyB->getCorners();
		Math::Vector2 pCenter = bodyB->getPosition();
		Math::Vector2 cCenter = bodyA->getPosition();
		float radius = bodyA->getRadius();
		Math::Vector2 cp;

		float minDistSqr = FLT_MAX;

		for (unsigned int i = 0; i < corners.size(); i++)
		{
			int j = (i + 1) % corners.size();
			Math::Vector2 va = corners[i];
			Math::Vector2 vb = corners[j];

			Math::Vector2 contact;
			float distSqr = Math::distPointToLine(cCenter, va, vb, &contact);
			if (distSqr < minDistSqr)
			{
				cp = contact;
				minDistSqr = distSqr;
			}
		}

		return cp;
	}

	Math::Vector2 eq::Physics::getContactLineCircle(Shape* bodyA, Shape* bodyB)
	{
		LineShape* line = static_cast<LineShape*>(bodyA);
		CircleShape* circle = static_cast<CircleShape*>(bodyB);

		//std::vector<Math::Vector2> corners = bodyB->getCorners();
		Math::Vector2 pCenter = line->getPosition();
		Math::Vector2 cCenter = circle->getPosition();
		float radius = circle->getRadius();
		Math::Vector2 cp;

		float minDistSqr = FLT_MAX;


		Math::Vector2 va = line->getStartPosition();
		Math::Vector2 vb = line->getEndPosition();

		Math::Vector2 contact;
		float distSqr = Math::distPointToLine(cCenter, va, vb, &contact);
		if (distSqr < minDistSqr)
		{
			cp = contact;
			minDistSqr = distSqr;
		}

		return cp;
	}

	Math::Vector2 eq::Physics::getContactLinePolygon(Shape* bodyA, Shape* bodyB)
	{
		LineShape* line = static_cast<LineShape*>(bodyA);
		PolygonShape* poly = static_cast<PolygonShape*>(bodyB);


		std::vector<Math::Vector2> pointsA;// = bodyA->getCorners();
		pointsA.push_back(line->getStartPosition());
		pointsA.push_back(line->getEndPosition());
		std::vector<Math::Vector2> pointsB = poly->getCorners();
		unsigned int contactCount = 0;

		Math::Vector2 contact1;
		Math::Vector2 contact2;

		float minDistSqr = FLT_MAX;

		Math::Vector2 closest;

		for (unsigned int i = 0; i < pointsA.size(); i++)
		{

			Math::Vector2 p = pointsA[i];

			for (unsigned int j = 0; j < pointsB.size(); j++)
			{
				Math::Vector2 a = pointsB[j];
				Math::Vector2 b = pointsB[(j + 1) % pointsB.size()];

				float distSqr = Math::distPointToLine(p, a, b, &closest);

				if (Math::nearlyEqual(distSqr, minDistSqr))
				{
					if (!nearlyEqual(closest, contact1))
					{
						contactCount = 2;
						contact2 = closest;
					}
				}
				else if (distSqr < minDistSqr)
				{
					minDistSqr = distSqr;
					contactCount = 1;
					contact1 = closest;
				}
			}

		}

		for (unsigned int i = 0; i < pointsB.size(); i++)
		{

			Math::Vector2 p = pointsB[i];


			Math::Vector2 a = pointsA[0];
			Math::Vector2 b = pointsA[1];

			float distSqr = Math::distPointToLine(p, a, b, &closest);

			if (Math::nearlyEqual(distSqr, minDistSqr))
			{
				if (!nearlyEqual(closest, contact1))
				{
					contactCount = 2;
					contact2 = closest;
				}
			}
			else if (distSqr < minDistSqr)
			{
				minDistSqr = distSqr;
				contactCount = 1;
				contact1 = closest;
			}

		}

		Math::Vector2 contact = contact1;
		if (contactCount == 2) contact = (contact1 + contact2) / 2;

		return contact;
	}

	Math::Vector2 eq::Physics::getContactLineBox(Shape* bodyA, Shape* bodyB)
	{
		LineShape* line = static_cast<LineShape*>(bodyA);
		BoxShape* poly = static_cast<BoxShape*>(bodyB);


		std::vector<Math::Vector2> pointsA;// = bodyA->getCorners();
		pointsA.push_back(line->getStartPosition());
		pointsA.push_back(line->getEndPosition());
		std::vector<Math::Vector2> pointsB = poly->getCorners();
		unsigned int contactCount = 0;

		Math::Vector2 contact1;
		Math::Vector2 contact2;

		float minDistSqr = FLT_MAX;

		Math::Vector2 closest;

		for (unsigned int i = 0; i < pointsA.size(); i++)
		{

			Math::Vector2 p = pointsA[i];

			for (unsigned int j = 0; j < pointsB.size(); j++)
			{
				Math::Vector2 a = pointsB[j];
				Math::Vector2 b = pointsB[(j + 1) % pointsB.size()];

				float distSqr = Math::distPointToLine(p, a, b, &closest);

				if (Math::nearlyEqual(distSqr, minDistSqr))
				{
					if (!nearlyEqual(closest, contact1))
					{
						contactCount = 2;
						contact2 = closest;
					}
				}
				else if (distSqr < minDistSqr)
				{
					minDistSqr = distSqr;
					contactCount = 1;
					contact1 = closest;
				}
			}

		}

		for (unsigned int i = 0; i < pointsB.size(); i++)
		{

			Math::Vector2 p = pointsB[i];


			Math::Vector2 a = pointsA[0];
			Math::Vector2 b = pointsA[1];

			float distSqr = Math::distPointToLine(p, a, b, &closest);

			if (Math::nearlyEqual(distSqr, minDistSqr))
			{
				if (!nearlyEqual(closest, contact1))
				{
					contactCount = 2;
					contact2 = closest;
				}
			}
			else if (distSqr < minDistSqr)
			{
				minDistSqr = distSqr;
				contactCount = 1;
				contact1 = closest;
			}

		}

		Math::Vector2 contact = contact1;
		if (contactCount == 2) contact = (contact1 + contact2) / 2;

		return contact;
	}
}