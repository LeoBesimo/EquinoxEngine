#include "../../Equinox/Physics/Shapes/PolygonShape.hpp"

#include "../../Equinox/Physics/Shapes/BoxShape.hpp"
#include "../../Equinox/Physics/Shapes/CircleShape.hpp"
#include "../../Equinox/Physics/Shapes/LineShape.hpp"

namespace eq
{
	namespace Physics
	{

		class LineShape;
		class BoxShape;
		class CircleShape;

		PolygonShape::PolygonShape(Math::Vector2 position, float angle, float sides, Material material, Math::Matrix2x2 scale) :
			Shape(position, angle, ShapeType::Polygon, material, scale), m_Sides(sides)
		{
			Math::Matrix2x2 transform = getScale() * Math::Matrix2x2(getAngle());
			float ac = Math::TWO_PI / sides;
			float maxAngle = sides <= 6 ? Math::TWO_PI : Math::TWO_PI - ac;
			int count = 0;
			for (float i = 0; i < maxAngle && count < sides; i += ac)
			{
				Math::Vector2 originalPoint(cos(i), sin(i));
				m_Original.push_back(originalPoint);
				m_Transformed.push_back(transform * originalPoint + getPosition());
				count++;
			}
			calculateUnits();
			calculateBoundingBox();
		}

		void PolygonShape::update(float delta)
		{
			applyGravity();
			Shape::update(delta);
			transformPoints();
		}

		void PolygonShape::update(float delta, int timeSteps)
		{
			applyGravity(timeSteps);
			Shape::update(delta, timeSteps);
			transformPoints();
		}

		void PolygonShape::transformPoints()
		{
			Math::Matrix2x2 rotation = Math::Matrix2x2(getAngle());
			for (unsigned int i = 0; i < m_Original.size(); i++)
			{
				Math::Matrix2x2 transform = rotation * getScale();
				m_Transformed[i] = transform * m_Original[i] + getPosition();
			}
		}

		void PolygonShape::move(Math::Vector2 distance)
		{
			Shape::move(distance);
			transformPoints();
		}

		Manifold PolygonShape::detectCollision(Shape* other)
		{
			return other->collidePolygon(this);
		}

		Manifold PolygonShape::collideLine(Shape* other)
		{
			LineShape* line = static_cast<LineShape*>(other);

			Manifold m;
			m.bodyA = this;
			m.bodyB = line;

			std::vector<Math::Vector2> normalsPoly = getNormals(getCorners());
			Math::Vector2 lineVector = line->getEndPosition() - line->getStartPosition();
			Math::Vector2 normalLine = Math::Vector2(-lineVector.y, lineVector.x);

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly.size(); i++)
			{
				Math::Vector2 projectionA = getMinMax(getCorners(), normalsPoly[i]);
				float dotLineA = Math::dot(line->getStartPosition(), normalsPoly[i]);
				float dotLineB = Math::dot(line->getEndPosition(), normalsPoly[i]);
				Math::Vector2 projectionB = Math::Vector2(std::min(dotLineA, dotLineB), std::max(dotLineA, dotLineB));//getMinMax(bodyB->getCorners(), normalsPoly1[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
				if (separated) break;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepth)
				{
					minDepth = depth;
					normal = normalsPoly[i];
				}
			}

			if (!separated)
			{
				Math::Vector2 projectionA = getMinMax(getCorners(), normalLine);
				float dotLineA = Math::dot(line->getStartPosition(), normalLine);
				float dotLineB = Math::dot(line->getEndPosition(), normalLine);
				Math::Vector2 projectionB = Math::Vector2(std::min(dotLineA, dotLineB), std::max(dotLineA, dotLineB));//getMinMax(bodyB->getCorners(), normalsPoly1[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepth)
				{
					minDepth = depth;
					normal = normalLine;
				}
			}

			m.colliding = !separated;

			if (!separated)
			{
				Math::Vector2 ab = line->getPosition() - getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactLinePolygon(line, this);
			}

			return m;
		}

		Manifold PolygonShape::collideCircle(Shape* other)
		{
			CircleShape* bodyA = static_cast<CircleShape*>(other);

			Manifold m;

			m.bodyA = bodyA;
			m.bodyB = this;

			std::vector<Math::Vector2> normalsPoly = getNormals(getCorners());

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly.size(); i++)
			{
				Math::Vector2 projectionA = getMinMaxCircle(bodyA->getPosition(), bodyA->getRadius(), normalsPoly[i]);
				Math::Vector2 projectionB = getMinMax(getCorners(), normalsPoly[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
				if (separated) break;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepth)
				{
					minDepth = depth;
					normal = normalsPoly[i];
				}
			}

			Math::Vector2 closestPoint = getClosestPoint(bodyA->getPosition(), getCorners());
			Math::Vector2 axis = closestPoint - bodyA->getPosition();

			Math::Vector2 projectionA = getMinMaxCircle(bodyA->getPosition(), bodyA->getRadius(), axis);
			Math::Vector2 projectionB = getMinMax(getCorners(), axis);

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
				Math::Vector2 ab = getPosition() - bodyA->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactCirclePolygon(bodyA, this);
			}

			return m;
		}

		Manifold PolygonShape::collidePolygon(Shape* other)
		{
			PolygonShape* bodyB = static_cast<PolygonShape*>(other);

			Manifold m;

			m.bodyA = this;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly1 = getNormals(getCorners());
			std::vector<Math::Vector2> normalsPoly2 = getNormals(bodyB->getCorners());

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly1.size(); i++)
			{
				Math::Vector2 projectionA = getMinMax(getCorners(), normalsPoly1[i]);
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
					Math::Vector2 projectionA = getMinMax(getCorners(), normalsPoly2[i]);
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
				Math::Vector2 ab = bodyB->getPosition() - getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactPolygonPolygon(this, bodyB);
			}

			return m;
		}

		Manifold PolygonShape::collideBox(Shape* other)
		{
			BoxShape* bodyA = static_cast<BoxShape*>(other);

			Manifold m;

			m.bodyA = bodyA;
			m.bodyB = this;

			std::vector<Math::Vector2> normalsPoly1 = getNormals(bodyA->getCorners());
			std::vector<Math::Vector2> normalsPoly2 = getNormals(getCorners());

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
				Math::Vector2 projectionB = getMinMax(getCorners(), normalsPoly1[i]);

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
					Math::Vector2 projectionB = getMinMax(getCorners(), normalsPoly2[i]);


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

				Math::Vector2 ab = getPosition() - bodyA->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				//m.penetration = minDepth / normal.len();

				//Math::Vector2 penetration = normal.normalize() * minDepth;

				//m.penetration = penetration.len();

				m.normal = normal.normalize();
				m.contact = getContactBoxPolygon(bodyA, this);
			}

			return m;
		}

		void PolygonShape::applyGravity()
		{
			applyForce(getGravit() * getMass());

			float factor = 1 / m_Transformed.size();

			for (unsigned int i = 0; i < m_Transformed.size(); i++)
			{
				Math::Vector2 radius = m_Transformed[i] - getPosition();
				applyForce(getGravit() * getInertia() * factor, radius);
			}
		}

		void PolygonShape::applyGravity(int timeSteps)
		{
			applyForce(getGravit() * getMass() / timeSteps);

			float factor = 1 / m_Transformed.size();

			for (unsigned int i = 0; i < m_Transformed.size(); i++)
			{
				Math::Vector2 radius = m_Transformed[i] - getPosition();
				applyForce(getGravit() * getInertia() * factor / timeSteps, radius);
			}
		}

		void PolygonShape::calculateBoundingBox()
		{
			float longestX = 0;
			float longestY = 0;
			Math::Vector2 position = getPosition();
			for (unsigned int i = 0; i < m_Transformed.size(); i++)
			{
				float x = abs(position.x - m_Transformed[i].x);
				float y = abs(position.y - m_Transformed[i].y);
				if (x > longestX) longestX = x;
				if (y > longestY) longestY = y;
			}

			float longest = std::max(longestX, longestY);
			Math::Vector2 min = Math::Vector2(-longest, -longest);
			Math::Vector2 max = Math::Vector2(longest, longest);
			setBoundingMin(min);
			setBoundingMax(max);
		}

		void PolygonShape::calculateUnits()
		{
			int n = m_Transformed.size();
			Math::Vector2 center = getPosition();

			Material material = getMaterial();

			float totalMass = 0;
			float totalInertia = 0;

			for (unsigned int i = 0; i < n; i++)
			{
				Math::Vector2 p1 = m_Transformed[i];
				Math::Vector2 p2 = m_Transformed[(i + 1) % n];

				Math::Vector2 midpoint;
				float heightSqr = Math::distPointToLine(center, p1, p2, &midpoint);
				float height = sqrt(heightSqr);

				Math::Vector2 triangleCenter1 = (center + p1 + midpoint) / 3;
				Math::Vector2 triangleCenter2 = (center + p2 + midpoint) / 3;

				float b1 = (p1 - midpoint).len();
				float b2 = (p2 - midpoint).len();

				float area1 = height * b1 / 2;
				float area2 = height * b2 / 2;

				float mass1 = area1 * material.density;
				float mass2 = area2 * material.density;

				float j1 = (mass1 / 18) * (b1 + heightSqr);
				float j2 = (mass2 / 18) * (b2 + heightSqr);

				totalMass += mass1;
				totalMass += mass2;

				totalInertia += j1 * (triangleCenter1 - center).len();//Math::pythagoreanSolve(center, triangleCenter1);
				totalInertia += j2 * (triangleCenter2 - center).len();//Math::pythagoreanSolve(center, triangleCenter2);
			}

			setMass(totalMass);
			setInertia(totalInertia);
		}
	}
}