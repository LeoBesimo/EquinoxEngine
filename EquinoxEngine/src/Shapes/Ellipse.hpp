#pragma once

namespace eq
{
	class Ellipse :
		public Drawable
	{
	private:
		float m_Radius1, m_Radius2;

	public:
		Ellipse(Math::Vector2 position, float radius1, float radius2) :
			m_Radius1(radius1), m_Radius2(radius2), Drawable(position, DrawableType::ELLIPSE)
		{}

		Ellipse(Math::Vector2 position, float radius) :
			m_Radius1(radius), Drawable(position, DrawableType::CIRCLE)
		{}

		Ellipse(Math::Vector2 position, float radius1, float radius2, Color& color) :
			m_Radius1(radius1), m_Radius2(radius2), Drawable(position, DrawableType::ELLIPSE, color)
		{}

		Ellipse(Math::Vector2 position, float radius, Color& color) :
			m_Radius1(radius), Drawable(position, DrawableType::CIRCLE, color)
		{}

		void getRadius(float* radius1, float* radius2) { *radius1 = m_Radius1; *radius2 = m_Radius2; }
		void getRadius(float* radius) { *radius = m_Radius1; }

		void setRadius(float radius) { m_Radius1 = radius; }
		void setRadius(float radius1, float radius2) { m_Radius1 = radius1; m_Radius2 = radius2; }
	};
}