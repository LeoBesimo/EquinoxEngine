#include "LineShape.hpp"

namespace eq
{
	namespace Physics
	{
		eq::Physics::LineShape::LineShape(Math::Vector2 startPos, Math::Vector2 endPos, Material material) :
			Shape(startPos + endPos / 2, 0, ShapeType::Line, material, Math::Matrix2x2(1, 0, 0, 1))
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
			return Manifold();
		}

		Manifold eq::Physics::LineShape::collideBox(Shape* other)
		{
			return Manifold();
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