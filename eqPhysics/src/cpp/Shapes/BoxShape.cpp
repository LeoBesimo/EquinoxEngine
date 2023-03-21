#include "../../Equinox/Physics/Shapes/BoxShape.hpp"

#include "../../Equinox/Physics/Shapes/PolygonShape.hpp"
#include "../../Equinox/Physics/Shapes/CircleShape.hpp"
#include "../../Equinox/Physics/Shapes/LineShape.hpp"
#include "../../Equinox/Physics/Shapes/CollisionUtilities.hpp"

namespace eq
{
	namespace Physics
	{
		class CircleShape;
		class PolygonShape;
		class LineShape;

		BoxShape::BoxShape(Math::Vector2 position, float angle, Material material, Math::Matrix2x2 scale) :
			Shape(position, angle, ShapeType::Box, material, scale)
		{
			m_Original[0] = Math::Vector2(-1, -1);
			m_Original[1] = Math::Vector2(1, -1);
			m_Original[2] = Math::Vector2(1, 1);
			m_Original[3] = Math::Vector2(-1, 1);
			m_Transformed.reserve(4);

			for (unsigned int i = 0; i < 4; i++)
				m_Transformed.push_back(Math::Vector2());

			transformPoints();
			calculateUnits();
			calculateBoundingBox();
		}

		void BoxShape::update(float delta)
		{
			applyGravity();
			Shape::update(delta);
			transformPoints();
		}

		void BoxShape::update(float delta, int timeSteps)
		{
			applyGravity(timeSteps);
			Shape::update(delta, timeSteps);
			transformPoints();
		}

		void BoxShape::transformPoints()
		{
			Math::Matrix2x2 rotation(getAngle());
			Math::Matrix2x2 transform = rotation * getScale();

			m_Transformed[0] = transform * m_Original[0] + getPosition();
			m_Transformed[1] = transform * m_Original[1] + getPosition();
			m_Transformed[2] = transform * m_Original[2] + getPosition();
			m_Transformed[3] = transform * m_Original[3] + getPosition();
		}

		void BoxShape::move(Math::Vector2 distance)
		{
			Shape::move(distance);
			transformPoints();
		}

		Manifold BoxShape::detectCollision(Shape* other)
		{
			return other->collideBox(this);
		}

		Manifold BoxShape::collideLine(Shape* other)
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
				m.contact = getContactLineBox(line, this);
			}

			return m;
		}

		Manifold BoxShape::collideCircle(Shape* other)
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
				Math::Vector2 ab = getPosition() - bodyA->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactCircleBox(bodyA, this);
				//OutputDebugString(L"Colliding");
			}

			return m;
		}

		Manifold BoxShape::collidePolygon(Shape* other)
		{
			PolygonShape* bodyB = static_cast<PolygonShape*>(other);

			Manifold m;

			m.bodyA = this;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly1 = getNormals(getCorners());
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
				Math::Vector2 projectionA = getMinMax(getCorners(), normalsPoly1[i]);
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
					Math::Vector2 projectionA = getMinMax(getCorners(), normalsPoly2[i]);
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

				Math::Vector2 ab = bodyB->getPosition() - getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				//m.penetration = minDepth / normal.len();

				//Math::Vector2 penetration = normal.normalize() * minDepth;

				//m.penetration = penetration.len();

				m.normal = normal.normalize();
				m.contact = getContactBoxPolygon(this, bodyB);
			}

			return m;
		}

		Manifold BoxShape::collideBox(Shape* other)
		{
			BoxShape* bodyB = static_cast<BoxShape*>(other);

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

			m.colliding = false;

			if (!separated)
			{
				Math::Vector2 ab = bodyB->getPosition() - getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactBoxBox(this, bodyB);
				m.colliding = true;
			}


			return m;
		}

		void BoxShape::applyGravity()
		{
			applyForce(getGravit() * getMass());

			for (uint8_t i = 0; i < 4; i++)
			{
				Math::Vector2 radius = m_Transformed[i] - getPosition();
				applyForce(getGravit() * getInertia() * 0.25, radius);
			}
		}

		void BoxShape::applyGravity(int timeSteps)
		{
			applyForce(getGravit() * getMass() / timeSteps);

			for (uint8_t i = 0; i < 4; i++)
			{
				Math::Vector2 radius = m_Transformed[i] - getPosition();
				applyForce(getGravit() * getInertia() * 0.25 / timeSteps, radius);
			}
		}

		void BoxShape::calculateBoundingBox()
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

		void BoxShape::calculateUnits()
		{
			Math::Matrix2x2 scale = getScale();
			float width = scale.a.x + scale.a.x;
			float height = scale.b.y + scale.b.y;
			float area = width * height;
			setMass(area * getMaterial().density);
			setInertia((1.f / 12.f) * getMass() * (width * width + height * height));
		}
	}
}