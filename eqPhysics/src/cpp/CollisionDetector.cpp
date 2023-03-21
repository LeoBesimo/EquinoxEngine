#include "../Equinox/Physics/CollisionDetector.hpp"
#include <algorithm>

namespace eq
{
	namespace Physics
	{
		Math::Vector2 CollisionDetector::getContactBoxBox(BoxShape* bodyA, BoxShape* bodyB)
		{
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

		Math::Vector2 CollisionDetector::getContactPolygonPolygon(PolygonShape* bodyA, PolygonShape* bodyB)
		{
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

		Math::Vector2 CollisionDetector::getContactBoxPolygon(BoxShape* bodyA, PolygonShape* bodyB)
		{
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

		Math::Vector2 CollisionDetector::getContactCircleBox(CircleShape* bodyA, BoxShape* bodyB)
		{
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

		Math::Vector2 CollisionDetector::getContactCirclePolygon(CircleShape* bodyA, PolygonShape* bodyB)
		{
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

		Math::Vector2 CollisionDetector::getContactLineCircle(LineShape* bodyA, CircleShape* bodyB)
		{
			return Math::Vector2();
		}

		Math::Vector2 CollisionDetector::getContactLinePolygon(LineShape* bodyA, PolygonShape* bodyB)
		{
			return Math::Vector2();
		}

		Math::Vector2 CollisionDetector::getContactLineBox(LineShape* bodyA, BoxShape* bodyB)
		{
			return Math::Vector2();
		}

		std::vector<Math::Vector2> CollisionDetector::getNormals(std::vector<Math::Vector2> corners)
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

		Math::Vector2 CollisionDetector::getMinMax(std::vector<Math::Vector2> corners, Math::Vector2 normal)
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

		Math::Vector2 CollisionDetector::getMinMaxCircle(Math::Vector2 center, float radius, Math::Vector2 normal)
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

		Math::Vector2 CollisionDetector::getClosestPoint(Math::Vector2 center, std::vector<Math::Vector2> points)
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

		Manifold CollisionDetector::CircleCircleCollision(CircleShape* bodyA, CircleShape* bodyB)
		{
			Manifold manifold;

			Math::Vector2 distanceVector = (bodyB->getPosition() - bodyA->getPosition());
			float distance = distanceVector.lenSqr();
			float radii = bodyA->getRadius() + bodyB->getRadius();

			if (distance >= (radii * radii))
			{
				manifold.colliding = false;
				return manifold;
			}

			manifold.colliding = true;
			manifold.bodyA = bodyA;
			manifold.bodyB = bodyB;
			manifold.penetration = radii - distanceVector.len();
			manifold.normal = distanceVector.normalize();
			manifold.contact = bodyA->getPosition() + (manifold.normal * (bodyA->getRadius() - manifold.penetration));

			return manifold;
		}

		Manifold CollisionDetector::PolygonPolygonCollision(PolygonShape* bodyA, PolygonShape* bodyB)
		{
			Manifold m;

			m.bodyA = bodyA;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly1 = getNormals(bodyA->getCorners());
			std::vector<Math::Vector2> normalsPoly2 = getNormals(bodyB->getCorners());

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly1.size(); i++)
			{
				Math::Vector2 projectionA = getMinMax(bodyA->getCorners(), normalsPoly1[i]);
				Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), normalsPoly1[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
				if (separated) break;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepth)
				{
					minDepth = depth;
					normal = normalsPoly1[i];
				}
			}

			if (!separated)
			{
				for (unsigned int i = 0; i < normalsPoly2.size(); i++)
				{
					Math::Vector2 projectionA = getMinMax(bodyA->getCorners(), normalsPoly2[i]);
					Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), normalsPoly2[i]);


					separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
					if (separated) break;

					float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

					if (depth < minDepth)
					{
						minDepth = depth;
						normal = normalsPoly2[i];
					}
				}
			}

			m.colliding = !separated;

			if (!separated)
			{
				Math::Vector2 ab = bodyB->getPosition() - bodyA->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactPolygonPolygon(bodyA, bodyB);
			}

			return m;
		}

		Manifold CollisionDetector::BoxBoxCollision(BoxShape* bodyA, BoxShape* bodyB)
		{
			Manifold m;

			m.bodyA = bodyA;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly1 = getNormals(bodyA->getCorners());
			std::vector<Math::Vector2> normalsPoly2 = getNormals(bodyB->getCorners());

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly1.size(); i++)
			{
				Math::Vector2 projectionA = getMinMax(bodyA->getCorners(), normalsPoly1[i]);
				Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), normalsPoly1[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
				if (separated) break;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepth)
				{
					minDepth = depth;
					normal = normalsPoly1[i];
				}
			}

			if (!separated)
			{
				for (unsigned int i = 0; i < normalsPoly2.size(); i++)
				{
					Math::Vector2 projectionA = getMinMax(bodyA->getCorners(), normalsPoly2[i]);
					Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), normalsPoly2[i]);


					separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
					if (separated) break;

					float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

					if (depth < minDepth)
					{
						minDepth = depth;
						normal = normalsPoly2[i];
					}
				}
			}

			m.colliding = false;

			if (!separated)
			{
				Math::Vector2 ab = bodyB->getPosition() - bodyA->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactBoxBox(bodyA, bodyB);
				m.colliding = true;
			}


			return m;
		}

		Manifold CollisionDetector::CircleBoxCollison(CircleShape* bodyA, BoxShape* bodyB)
		{
			Manifold m;

			m.bodyA = bodyA;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly = getNormals(bodyB->getCorners());

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly.size(); i++)
			{
				Math::Vector2 projectionA = getMinMaxCircle(bodyA->getPosition(), bodyA->getRadius(), normalsPoly[i]);
				Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), normalsPoly[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
				if (separated) break;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepth)
				{
					minDepth = depth;
					normal = normalsPoly[i];
				}
			}

			Math::Vector2 closestPoint = getClosestPoint(bodyA->getPosition(), bodyB->getCorners());
			Math::Vector2 axis = closestPoint - bodyA->getPosition();

			Math::Vector2 projectionA = getMinMaxCircle(bodyA->getPosition(), bodyA->getRadius(), axis);
			Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), axis);

			separated = (projectionA.x >= projectionB.y || projectionB.x >= projectionA.y);

			if (separated)
			{
				m.colliding = false;
				return m;
			}

			float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

			if (depth < minDepth)
			{
				minDepth = depth;
				normal = axis;
			}

			m.colliding = !separated;
			if (!separated)
			{
				Math::Vector2 ab = bodyB->getPosition() - bodyA->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactCircleBox(bodyA, bodyB);
				//OutputDebugString(L"Colliding");
			}

			return m;
		}

		Manifold CollisionDetector::CirclePolygonCollison(CircleShape* bodyA, PolygonShape* bodyB)
		{
			Manifold m;

			m.bodyA = bodyA;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly = getNormals(bodyB->getCorners());

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly.size(); i++)
			{
				Math::Vector2 projectionA = getMinMaxCircle(bodyA->getPosition(), bodyA->getRadius(), normalsPoly[i]);
				Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), normalsPoly[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
				if (separated) break;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepth)
				{
					minDepth = depth;
					normal = normalsPoly[i];
				}
			}

			Math::Vector2 closestPoint = getClosestPoint(bodyA->getPosition(), bodyB->getCorners());
			Math::Vector2 axis = closestPoint - bodyA->getPosition();

			Math::Vector2 projectionA = getMinMaxCircle(bodyA->getPosition(), bodyA->getRadius(), axis);
			Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), axis);

			separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;

			float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

			if (depth < minDepth)
			{
				minDepth = depth;
				normal = axis;
			}

			m.colliding = !separated;
			if (!separated)
			{
				Math::Vector2 ab = bodyB->getPosition() - bodyA->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactCirclePolygon(bodyA, bodyB);
			}

			return m;
		}

		Manifold CollisionDetector::BoxPolygonCollision(BoxShape* bodyA, PolygonShape* bodyB)
		{
			Manifold m;

			m.bodyA = bodyA;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly1 = getNormals(bodyA->getCorners());
			std::vector<Math::Vector2> normalsPoly2 = getNormals(bodyB->getCorners());

			bool separated = false;

			Math::Vector2 normalA;
			Math::Vector2 normalB;
			Math::Vector2 normal;
			float minDepthA = FLT_MAX;
			float minDepthB = FLT_MAX;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly1.size(); i++)
			{
				Math::Vector2 projectionA = getMinMax(bodyA->getCorners(), normalsPoly1[i]);
				Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), normalsPoly1[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
				if (separated) break;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepthA)
				{
					minDepthA = depth;
					normalA = normalsPoly1[i];
				}
			}

			if (!separated)
			{
				for (unsigned int i = 0; i < normalsPoly2.size(); i++)
				{
					Math::Vector2 projectionA = getMinMax(bodyA->getCorners(), normalsPoly2[i]);
					Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), normalsPoly2[i]);


					separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
					if (separated) break;

					float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

					if (depth < minDepthB)
					{
						minDepthB = depth;
						normalB = normalsPoly2[i];
					}
				}
			}

			m.colliding = !separated;

			if (!separated)
			{
				float penetrationA = minDepthA / normalA.len();
				float penetrationB = minDepthB / normalB.len();

				if (penetrationA < penetrationB)
				{
					m.penetration = penetrationA;
					normal = normalA;
				}
				else
				{
					m.penetration = penetrationB;
					normal = normalB;
				}

				Math::Vector2 ab = bodyB->getPosition() - bodyA->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				//m.penetration = minDepth / normal.len();

				//Math::Vector2 penetration = normal.normalize() * minDepth;

				//m.penetration = penetration.len();

				m.normal = normal.normalize();
				m.contact = getContactBoxPolygon(bodyA, bodyB);
			}

			return m;
		}

		Manifold CollisionDetector::LineCircleCollision(LineShape* bodyA, CircleShape* bodyB)
		{
			Manifold m;
			m.bodyA = bodyA;
			m.bodyB = bodyB;
			return m;
		}

		Manifold CollisionDetector::LinePolygonCollision(LineShape* bodyA, PolygonShape* bodyB)
		{
			Manifold m;
			m.bodyA = bodyA;
			m.bodyB = bodyB;
			return m;
		}

		Manifold CollisionDetector::LineBoxCollision(LineShape* bodyA, BoxShape* bodyB)
		{
			Manifold m;
			m.bodyA = bodyA;
			m.bodyB = bodyB;
			return m;
		}

		Manifold CollisionDetector::LineLineCollision(LineShape* bodyA, LineShape* bodyB)
		{
			return Manifold();
		}

		CollisionDetector::CollisionDetector()
		{}

		bool CollisionDetector::boundingBoxCollision(Shape* bodyA, Shape* bodyB)
		{
			Math::Vector2 bodyAMin = bodyA->getPosition() + bodyA->getBoundingMin();
			Math::Vector2 bodyAMax = bodyA->getPosition() + bodyA->getBoundingMax();
			Math::Vector2 bodyBMin = bodyB->getPosition() + bodyB->getBoundingMin();
			Math::Vector2 bodyBMax = bodyB->getPosition() + bodyB->getBoundingMax();
			return !(bodyAMax.x < bodyBMin.x || bodyAMin.x > bodyBMax.x || bodyAMax.y < bodyBMin.y || bodyAMin.y > bodyBMax.y);
		}

		Manifold CollisionDetector::detectCollision(Shape* bodyA, Shape* bodyB)
		{
			switch (bodyA->getShapeType())
			{
			case ShapeType::Box:
			{
				switch (bodyB->getShapeType())
				{
				case ShapeType::Box:
				{
					return BoxBoxCollision(static_cast<BoxShape*>(bodyA), static_cast<BoxShape*>(bodyB));
				}break;

				case ShapeType::Circle:
				{
					return CircleBoxCollison(static_cast<CircleShape*>(bodyB), static_cast<BoxShape*>(bodyA));
				}break;

				case ShapeType::Polygon:
				{
					return BoxPolygonCollision(static_cast<BoxShape*>(bodyA), static_cast<PolygonShape*>(bodyB));
				}break;

				case ShapeType::Line:
				{
					return LineBoxCollision(static_cast<LineShape*>(bodyB), static_cast<BoxShape*>(bodyA));
				}break;
				}
			}break;

			case ShapeType::Circle:
			{
				switch (bodyB->getShapeType())
				{
				case ShapeType::Box:
				{
					return CircleBoxCollison(static_cast<CircleShape*>(bodyA), static_cast<BoxShape*>(bodyB));
				}break;

				case ShapeType::Circle:
				{
					return CircleCircleCollision(static_cast<CircleShape*>(bodyA), static_cast<CircleShape*>(bodyB));
				}break;

				case ShapeType::Polygon:
				{
					return CirclePolygonCollison(static_cast<CircleShape*>(bodyA), static_cast<PolygonShape*>(bodyB));
				}break;

				case ShapeType::Line:
				{
					return LineCircleCollision(static_cast<LineShape*>(bodyB), static_cast<CircleShape*>(bodyA));
				}break;
				}
			}break;

			case ShapeType::Polygon:
			{
				switch (bodyB->getShapeType())
				{
				case ShapeType::Box:
				{
					return BoxPolygonCollision(static_cast<BoxShape*>(bodyB), static_cast<PolygonShape*>(bodyA));
				}break;

				case ShapeType::Circle:
				{
					return CirclePolygonCollison(static_cast<CircleShape*>(bodyB), static_cast<PolygonShape*>(bodyA));
				}break;

				case ShapeType::Polygon:
				{
					return PolygonPolygonCollision(static_cast<PolygonShape*>(bodyA), static_cast<PolygonShape*>(bodyB));
				}break;

				case ShapeType::Line:
				{
					return LinePolygonCollision(static_cast<LineShape*>(bodyB), static_cast<PolygonShape*>(bodyA));
				}break;
				}
			}break;
			case ShapeType::Line:
			{
				switch (bodyB->getShapeType())
				{
				case ShapeType::Box:
				{
					return LineBoxCollision(static_cast<LineShape*>(bodyA), static_cast<BoxShape*>(bodyB));
				}break;

				case ShapeType::Circle:
				{
					return LineCircleCollision(static_cast<LineShape*>(bodyA), static_cast<CircleShape*>(bodyB));
				}break;

				case ShapeType::Polygon:
				{
					return LinePolygonCollision(static_cast<LineShape*>(bodyA), static_cast<PolygonShape*>(bodyB));
				}break;

				case ShapeType::Line:
				{
					return LineLineCollision(static_cast<LineShape*>(bodyA), static_cast<LineShape*>(bodyB));
				}break;

				}break;
			}
			}
			Manifold m;
			m.colliding = false;
			return m;
		}
	}
}