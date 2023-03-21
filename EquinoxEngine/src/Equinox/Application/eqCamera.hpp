#pragma once

#include <Equinox/Math/eqMath.hpp>

namespace eq
{
	class Camera
	{
	private:
		Math::Vector2 m_Position;
		Math::Vector2 m_Dimension;
		Math::Matrix2x2 m_Transform;

	public:
		Camera() :
			m_Position(0, 0), m_Dimension(0, 0), m_Transform(1, 0, 0, 1)
		{}

		Math::Vector2 getPosition() { return m_Position; }
		Math::Vector2 getDimension() { return m_Dimension; }
		Math::Matrix2x2 getTransform() { return m_Transform; }
		void setPosition(Math::Vector2 position) { m_Position = position; }
		void setDimension(Math::Vector2 dimension) { m_Dimension = dimension; }
		void setTransform(Math::Matrix2x2 transform) { m_Transform = transform; }

		void move(Math::Vector2 direction) { m_Position += direction; }
	};
}