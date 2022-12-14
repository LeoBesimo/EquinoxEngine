#include "Equinox.hpp"
#include "Testgame.hpp"

equinoxAppEntryPoint
{
	Testgame game;

	eq::Renderer::setClearColor(eq::Color(255, 0, 0));

	eq::Application::setApplicationUpdate([&](float delta)
	{

		eq::Math::Vector2 mouse;
		eq::Input::getMousePosition(&mouse);

		game.m_Update(delta);

		eq::Renderer::DrawCircle(mouse, 10, eq::Color(0, 255, 0));
	});

	eq::Application::start();

	return 0;
}