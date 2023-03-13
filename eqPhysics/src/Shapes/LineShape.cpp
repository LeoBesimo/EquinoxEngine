#include "LineShape.hpp"

eq::Physics::LineShape::LineShape(Math::Vector2 startPos, Math::Vector2 endPos, Material material) :
	Shape(startPos + endPos / 2, 0, ShapeType::Line, material, Math::Matrix2x2(1, 0, 0, 1))
{
	m_StartPos = startPos;
	m_EndPos = endPos;
	setAngle((atan((endPos.y - startPos.y) / (endPos.x - startPos.x))));
	m_Length = (endPos - startPos).len();
}

eq::Physics::LineShape::LineShape(Math::Vector2 pos, float angle, float length, Material material) :
	Shape(pos, angle, ShapeType::Line, material, Math::Matrix2x2(1, 0, 0, 1))
{
	m_Length = length;
	Math::Vector2 offset = Math::Vector2(cos(angle), sin(angle));
	m_StartPos = pos - offset * length / 2;
	m_EndPos = pos + offset * length / 2;
}

void eq::Physics::LineShape::update(float delta)
{
}

void eq::Physics::LineShape::update(float delta, int timeSteps)
{
}

void eq::Physics::LineShape::applyGravity()
{
}

void eq::Physics::LineShape::applyGravity(int timeSteps)
{
}

void eq::Physics::LineShape::calculateBoundingBox()
{
}

void eq::Physics::LineShape::calculateUnits()
{
}
