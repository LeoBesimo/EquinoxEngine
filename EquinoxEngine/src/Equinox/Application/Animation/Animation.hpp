#pragma once

#include "../Images/Sprite.hpp"
#include "../Images/Texture.hpp"

#include <Equinox/Math/eqMath.hpp>


namespace eq
{
	class Animation
	{
	private:
		uint32_t m_FrameIndex;
		uint32_t m_Counter;
		uint32_t m_FramesPerAnimationFrame;
		std::vector<Sprite> m_AnimationFrames;

	public:
		Animation(const BitmapTexture& texture, uint32_t frameWidth, uint32_t frameHeight, uint32_t xOffset = 0, uint32_t yOffset = 0);

		void setPosition(Math::Vector2 position);
		void rotate(float angle);
		void scale(float xScale, float yScale);
		uint32_t getFrameCount();

		Sprite& getAnimationFrame();
	};
}