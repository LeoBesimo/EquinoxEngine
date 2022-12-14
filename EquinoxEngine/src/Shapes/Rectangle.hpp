#pragma once

#include "Drawable.hpp"

namespace eq
{
	class Rectangle :
		public Drawable
	{
	private:
		Math::Vector2 m_Dimension;

	public:
		Rectangle(Math::Vector2 position, Math::Vector2 dimension) :
			m_Dimension(dimension), Drawable(position, DrawableType::RECT)
		{}

		Rectangle(Math::Vector2 position, Math::Vector2 dimension, Color& color) :
			m_Dimension(dimension), Drawable(position, DrawableType::RECT, color)
		{}

		Math::Vector2 getDimension() { return m_Dimension; }
		void setDimension(Math::Vector2 dimension) { m_Dimension = dimension; }
	};
}