#include "../../Equinox/Application/Animation/Animation.hpp"

eq::Animation::Animation(const BitmapTexture& texture, uint32_t frameWidth, uint32_t frameHeight, uint32_t xOffset, uint32_t yOffset)
{

}

void eq::Animation::setPosition(Math::Vector2 position)
{
	for (uint32_t i = 0; i < m_AnimationFrames.size(); i++)
	{
		m_AnimationFrames[i].setPosition(position);
	}
}

void eq::Animation::rotate(float angle)
{
	for (uint32_t i = 0; i < m_AnimationFrames.size(); i++)
	{
		m_AnimationFrames[i].rotate(angle);
		m_AnimationFrames[i].preprocessSprite();
	}
}

void eq::Animation::scale(float xScale, float yScale)
{
	for (uint32_t i = 0; i < m_AnimationFrames.size(); i++)
	{
		m_AnimationFrames[i].scale(xScale,yScale);
		m_AnimationFrames[i].preprocessSprite();
	}
}

uint32_t eq::Animation::getFrameCount()
{
	return m_AnimationFrames.size();
}

eq::Sprite& eq::Animation::getAnimationFrame()
{
	m_Counter++;
	if (m_Counter >= m_FramesPerAnimationFrame)
	{
		m_Counter = 0;
		m_FrameIndex = (m_FrameIndex++) % m_AnimationFrames.size();
	}

	return m_AnimationFrames[m_FrameIndex];
}
