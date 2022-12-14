#pragma once
#include <Equinox.hpp>

class Testgame : public eq::Application
{
public:
	void m_Update(float delta)
	{
		OutputDebugString(L"Test\n");
	}

	void render()
	{

	}
};

