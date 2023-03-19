#include "LineShape.hpp"

#include "BoxShape.hpp"
#include "CircleShape.hpp"
#include "PolygonShape.hpp"

namespace eq
{
	namespace Physics
	{
		class BoxShape;
		class CircleShape;
		class PolygonShape;

		eq::Physics::LineShape::LineShape(Math::Vector2 startPos, Math::Vector2 endPos, Material material) :
			Shape((startPos + endPos) / 2, 0, ShapeType::Line, material, Math::Matrix2x2(1, 0, 0, 1))
		{
			m_StartPos = startPos;
			m_EndPos = endPos;
			setAngle((atan((endPos.y - startPos.y) / (endPos.x - startPos.x))));
			m_Length = (endPos - startPos).len();
			calculateUnits();
			calculateBoundingBox();
		}

		eq::Physics::LineShape::LineShape(Math::Vector2 pos, float angle, float length, Material material) :
			Shape(pos, angle, ShapeType::Line, material, Math::Matrix2x2(1, 0, 0, 1))
		{
			m_Length = length;
			Math::Vector2 offset = Math::Vector2(cos(angle), sin(angle));
			m_StartPos = pos - offset * length / 2;
			m_EndPos = pos + offset * length / 2;
			calculateUnits();
			calculateBoundingBox();
		}

		void eq::Physics::LineShape::update(float delta)
		{
			applyGravity();
			Shape::update(delta);
			m_StartPos = getPosition() - Math::Vector2::fromAngle(getAngle()) * m_Length / 2;
			m_EndPos = getPosition() + Math::Vector2::fromAngle(getAngle()) * m_Length / 2;
		}

		void eq::Physics::LineShape::update(float delta, int timeSteps)
		{
			applyGravity(timeSteps);
			Shape::update(delta, timeSteps);
		}

		eq::Physics::Manifold eq::Physics::LineShape::detectCollision(Shape* other)
		{
			return other->collideLine(this);
		}

		Manifold eq::Physics::LineShape::collideLine(Shape* other)
		{
			return Manifold();
		}

		Manifold eq::Physics::LineShape::collideCircle(Shape* other)
		{
			return Manifold();
		}

		Manifold eq::Physics::LineShape::collidePolygon(Shape* other)
		{
			PolygonShape* bodyB = static_cast<PolygonShape*>(other);

			Manifold m;
			m.bodyA = this;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly = getNormals(bodyB->getCorners());
			Math::Vector2 lineVector = getEndPosition() - getStartPosition();
			Math::Vector2 normalLine = Math::Vector2(-lineVector.y, lineVector.x);

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly.size(); i++)
			{
				Math::Vector2 projectionA = getMinMax(bodyB->getCorners(), normalsPoly[i]);
				float dotLineA = Math::dot(getStartPosition(), normalsPoly[i]);
				float dotLineB = Math::dot(getEndPosition(), normalsPoly[i]);
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
				Math::Vector2 projectionA = getMinMax(bodyB->getCorners(), normalLine);
				float dotLineA = Math::dot(getStartPosition(), normalLine);
				float dotLineB = Math::dot(getEndPosition(), normalLine);
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
				Math::Vector2 ab = bodyB->getPosition() - getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactLinePolygon(this, bodyB);
			}

			return m;
		}

		Manifold eq::Physics::LineShape::collideBox(Shape* other)
		{
			BoxShape* bodyB = static_cast<BoxShape*>(other);

			Manifold m;
			m.bodyA = this;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly = getNormals(bodyB->getCorners());
			Math::Vector2 lineVector = getEndPosition() - getStartPosition();
			Math::Vector2 normalLine = Math::Vector2(-lineVector.y, lineVector.x);

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly.size(); i++)
			{
				Math::Vector2 projectionA = getMinMax(bodyB->getCorners(), normalsPoly[i]);
				float dotLineA = Math::dot(getStartPosition(), normalsPoly[i]);
				float dotLineB = Math::dot(getEndPosition(), normalsPoly[i]);
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
				Math::Vector2 projectionA = getMinMax(bodyB->getCorners(), normalLine);
				float dotLineA = Math::dot(getStartPosition(), normalLine);
				float dotLineB = Math::dot(getEndPosition(), normalLine);
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
				Math::Vector2 ab = bodyB->getPosition() - getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactLineBox(this, bodyB);
			}

			return m;
		}

		void eq::Physics::LineShape::applyGravity()
		{
			applyForce(getGravit() * getMass());
		}

		void eq::Physics::LineShape::applyGravity(int timeSteps)
		{

		}

		void eq::Physics::LineShape::calculateBoundingBox()
		{
			setBoundingMin(eq::Math::Vector2(-m_Length / 2, -m_Length / 2));
			setBoundingMax(eq::Math::Vector2(m_Length / 2, m_Length / 2));
		}

		void eq::Physics::LineShape::calculateUnits()
		{
			setMass(m_Length * getMaterial().density);
			setInertia(getMass() * m_Length / 12);
		}
	}
}