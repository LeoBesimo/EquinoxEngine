#pragma once

#include <eqMath.hpp>

namespace eq
{
	class Camera
	{
	private:
		Math::Vector2 m_Position;
		Math::Vector2 m_Dimension;

	public:
		Camera() :
			m_Position(0, 0), m_Dimension(0, 0)
		{}

		Math::Vector2 getPosition() { return m_Position; }
		Math::Vector2 getDimension() { return m_Dimension; }
		void setPosition(Math::Vector2 position) { m_Position = position; }
		void setDimension(Math::Vector2 dimension) { m_Dimension = dimension; }

		void move(Math::Vector2 direction) { m_Position += direction; }
	};
}