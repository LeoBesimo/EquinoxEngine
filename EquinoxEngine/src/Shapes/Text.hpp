#pragma once

#include "Drawable.hpp"
#include <string>

namespace eq
{
	class Text :
		public Drawable
	{
	private:
		std::wstring m_Text;
		std::vector<wchar_t> chars;

	public:
		Text() :
			Drawable(Math::Vector2(), DrawableType::TEXT)
		{}

		Text(Math::Vector2 position, std::wstring text) :
			Drawable(position, DrawableType::TEXT)
		{
			m_Text = text;
			//chars = std::vector<wchar_t>(text.begin(), text.end());
		}

		Text(Math::Vector2 position, std::wstring& text, Color& color) :
			Drawable(position, DrawableType::TEXT, color)
		{
			m_Text = text;
		}

		std::wstring& getText() { return m_Text; }
		void setText(std::wstring& text) { m_Text = text; }
	};
}