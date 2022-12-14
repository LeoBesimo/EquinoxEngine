#include "Equinox.hpp"
#include "Testgame.hpp"

equinoxAppEntryPoint
{
	Testgame game;
	
	std::wstring t(L"Test");

	eq::Text text(eq::Math::Vector2(300, 300), t);

	eq::Renderer::setClearColor(eq::Color(255, 0, 0));
	eq::Application::setWindowProperties(L"Test", 800, 800);
	eq::Application::setApplicationUpdate([&](float delta)
	{

		eq::Math::Vector2 mouse;
		eq::Input::getMousePosition(&mouse);

		text.setPosition(mouse);

		game.update(delta);
		game.render();

		eq::Renderer::DrawCircle(mouse, 30, eq::Color(0, 255, 0));
		eq::Renderer::draw(std::make_shared<eq::Text>(text));
	});

	eq::Application::start();

	return 0;
}