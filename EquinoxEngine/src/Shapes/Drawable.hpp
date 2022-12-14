#pragma once

#include <eqMath.hpp>

#include "../eqRenderUtilities.hpp"

namespace eq
{
	enum class DrawableType
	{
		RECT, CIRCLE, ELLIPSE, LINE, TEXT, NONE
	};

	class Drawable
	{
	protected:
		Math::Vector2 m_Position;
		DrawableType m_Type;
		Color m_Color;

	public:
		Drawable() :
			m_Position(), m_Type(DrawableType::NONE), m_Color()
		{}

		Drawable(Math::Vector2 position, DrawableType type) :
			m_Position(position), m_Type(type)
		{}

		Drawable(Math::Vector2 position, DrawableType type, Color& color) :
			m_Position(position), m_Type(type), m_Color(color)
		{}

		void setPosition(Math::Vector2 position) { m_Position = position; }
		Math::Vector2 getPosition() { return m_Position; }

		void setColor(Color& color) { m_Color = color; }
		Color getColor() { return m_Color; }

		DrawableType getType() { return m_Type; }
	};
}