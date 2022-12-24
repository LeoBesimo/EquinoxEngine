#include "CircleShape.hpp"

namespace eq
{
	namespace Physics
	{
		CircleShape::CircleShape(Math::Vector2 position, float angle, float radius, Material material) :
			Shape(position, angle, ShapeType::Circle, material, Math::Matrix2x2()), m_Radius(radius)
		{
			calculateUnits();
		}

		void CircleShape::update(float delta)
		{
			/*wchar_t buffer[32];
			swprintf(buffer, 32, L"%f : %f\n", getAngle(), getOmega());
			OutputDebugString(buffer);*/
			applyGravity();
			Shape::update(delta);
		}

		void CircleShape::applyGravity()
		{
			setForce(getForce() + (getGravit() * getMass()));
		}

		void CircleShape::calculateUnits()
		{
			float area = m_Radius * m_Radius * Math::PI;
			setMass(area * getMaterial().density);
			setInertia(0.5f * getMass() * m_Radius * m_Radius);
		}
	}
}