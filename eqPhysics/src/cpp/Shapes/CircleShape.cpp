#include "../../Equinox/Physics/Shapes/CircleShape.hpp"

#include "../../Equinox/Physics/Shapes/PolygonShape.hpp"
#include "../../Equinox/Physics/Shapes/BoxShape.hpp"
#include "../../Equinox/Physics/Shapes/LineShape.hpp"
#include "../../Equinox/Physics/Shapes/CollisionUtilities.hpp"


namespace eq
{
	namespace Physics
	{
		class LineShape;
		class BoxShape;
		class PolygonShape;

		CircleShape::CircleShape(Math::Vector2 position, float angle, float radius, Material material) :
			Shape(position, angle, ShapeType::Circle, material, Math::Matrix2x2()), m_Radius(radius)
		{
			calculateUnits();
			calculateBoundingBox();
		}

		void CircleShape::update(float delta)
		{
			/*wchar_t buffer[32];
			swprintf(buffer, 32, L"%f : %f\n", getAngle(), getOmega());
			OutputDebugString(buffer);*/
			applyGravity();
			Shape::update(delta);
		}

		void CircleShape::update(float delta, int timeSteps)
		{
			applyGravity(timeSteps);
			Shape::update(delta, timeSteps);
		}

		Manifold CircleShape::detectCollision(Shape* other)
		{
			return other->collideCircle(this);
		}

		Manifold CircleShape::collideLine(Shape* other)
		{
			Manifold m;

			LineShape* line = static_cast<LineShape*>(other);

			m.bodyA = this;
			m.bodyB = line;

			Math::Vector2 lineVector = line->getEndPosition() - line->getStartPosition();
			Math::Vector2 lineNormal = Math::Vector2(-lineVector.y, lineVector.x);

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;


			Math::Vector2 projectionA = getMinMaxCircle(getPosition(), getRadius(), lineVector);
			float dotLineA = Math::dot(line->getStartPosition(), lineNormal);
			float dotLineB = Math::dot(line->getEndPosition(), lineNormal);
			Math::Vector2 projectionB = Math::Vector2(std::min(dotLineA, dotLineB), std::max(dotLineA, dotLineB));
			separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
			//if (separated) break;

			float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

			if (depth < minDepth)
			{
				minDepth = depth;
				normal = lineNormal;
			}

			std::vector<Math::Vector2> corners;
			corners.push_back(line->getStartPosition());
			corners.push_back(line->getEndPosition());

			Math::Vector2 closestPoint = getClosestPoint(getPosition(), corners);
			Math::Vector2 axis = closestPoint - getPosition();

			projectionA = getMinMaxCircle(getPosition(), getRadius(), axis);
			dotLineA = Math::dot(line->getStartPosition(), axis);
			dotLineB = Math::dot(line->getEndPosition(), axis);
			projectionB = Math::Vector2(std::min(dotLineA, dotLineB), std::max(dotLineA, dotLineB));

			separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;

			depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

			if (depth < minDepth)
			{
				minDepth = depth;
				normal = axis;
			}

			m.colliding = !separated;
			if (!separated)
			{
				Math::Vector2 ab = getPosition() - line->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactLineCircle(line, this);
			}

			return m;
		}

		Manifold CircleShape::collideCircle(Shape* other)
		{
			Manifold manifold;

			CircleShape* bodyB = static_cast<CircleShape*>(other);

			Math::Vector2 distanceVector = (bodyB->getPosition() - getPosition());
			float distance = distanceVector.lenSqr();
			float radii = getRadius() + bodyB->getRadius();

			if (distance >= (radii * radii))
			{
				manifold.colliding = false;
				return manifold;
			}

			manifold.colliding = true;
			manifold.bodyA = this;
			manifold.bodyB = bodyB;
			manifold.penetration = radii - distanceVector.len();
			manifold.normal = distanceVector.normalize();
			manifold.contact = getPosition() + (manifold.normal * (getRadius() - manifold.penetration));

			return manifold;
		}

		Manifold CircleShape::collidePolygon(Shape* other)
		{
			Manifold m;

			PolygonShape* bodyB = static_cast<PolygonShape*>(other);

			m.bodyA = this;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly = getNormals(bodyB->getCorners());

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly.size(); i++)
			{
				Math::Vector2 projectionA = getMinMaxCircle(getPosition(), getRadius(), normalsPoly[i]);
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

			Math::Vector2 closestPoint = getClosestPoint(getPosition(), bodyB->getCorners());
			Math::Vector2 axis = closestPoint - getPosition();

			Math::Vector2 projectionA = getMinMaxCircle(getPosition(), getRadius(), axis);
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
				Math::Vector2 ab = bodyB->getPosition() - getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactCirclePolygon(this, other);
			}

			return m;
		}

		Manifold CircleShape::collideBox(Shape* other)
		{
			BoxShape* bodyB = static_cast<BoxShape*>(other);
			Manifold m;

			m.bodyA = this;
			m.bodyB = bodyB;

			std::vector<Math::Vector2> normalsPoly = getNormals(bodyB->getCorners());

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly.size(); i++)
			{
				Math::Vector2 projectionA = getMinMaxCircle(getPosition(), getRadius(), normalsPoly[i]);
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

			Math::Vector2 closestPoint = getClosestPoint(getPosition(), bodyB->getCorners());
			Math::Vector2 axis = closestPoint - getPosition();

			Math::Vector2 projectionA = getMinMaxCircle(getPosition(), getRadius(), axis);
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
				Math::Vector2 ab = bodyB->getPosition() - getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactCircleBox(this, bodyB);
				//OutputDebugString(L"Colliding");
			}

			return m;
		}

		void CircleShape::applyGravity()
		{
			setForce(getForce() + (getGravit() * getMass()));
		}

		void CircleShape::applyGravity(int timeSteps)
		{
			setForce(getForce() + (getGravit() * getMass() / timeSteps));
		}

		void CircleShape::calculateBoundingBox()
		{
			static float scaler = 1.5;
			Math::Vector2 min = Math::Vector2(-m_Radius * scaler, -m_Radius * scaler);
			Math::Vector2 max = Math::Vector2(m_Radius * scaler, m_Radius * scaler);
			setBoundingMin(min);
			setBoundingMax(max);
		}

		void CircleShape::calculateUnits()
		{
			float area = m_Radius * m_Radius * Math::PI;
			setMass(area * getMaterial().density);
			setInertia(0.5f * getMass() * m_Radius * m_Radius);
		}
	}
}