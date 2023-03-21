#pragma once

#include "Drawable.hpp"

namespace eq
{
	class Line :
		public Drawable
	{
	private:
		Math::Vector2 m_Start;
		Math::Vector2 m_End;

	public:
		Line(Math::Vector2 p1, Math::Vector2 p2) :
			m_Start(p1), m_End(p2), Drawable(Math::Vector2(), DrawableType::LINE)
		{}

		Line(Math::Vector2 p1, Math::Vector2 p2, Color color) :
			m_Start(p1), m_End(p2), Drawable(Math::Vector2(), DrawableType::LINE, color)
		{}

		Math::Vector2 getStartPos() { return m_Start; }
		Math::Vector2 getEndPos() { return m_End; }

		void setStartPos(Math::Vector2 startPos) { m_Start = startPos; }
		void setEndPos(Math::Vector2 endPos) { m_End = endPos; }

		void getStartPos(int* x, int* y) { *x = (int)m_Start.x; *y = (int)m_Start.y; }
		void getEndPos(int* x, int* y) { *x = (int)m_End.x; *y = (int)m_End.y; };
	};
}