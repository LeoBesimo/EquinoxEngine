#include "Equinox.hpp"
#include "Testgame.hpp"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

equinoxAppEntryPoint
{
	Testgame game;

	std::wstring t(L"Test");

	eq::Text text(eq::Math::Vector2(0, 0), t);
	text.setCameraDependent(true);

	eq::Text text2(eq::Math::Vector2(100, 100), std::wstring(L"yeet"));

	eq::Rectangle rect(eq::Math::Vector2(200, 200), eq::Math::Vector2(100, 100), eq::Color(255,0,0));
	rect.setCameraDependent(true);

	eq::Ellipse circle(eq::Math::Vector2(300, 100), 30, eq::Color(0, 255, 0, 128));
	circle.setCameraDependent(false);

	eq::BitmapTexture texture(400,400);
	for (unsigned int i = 0; i < 400; i++)
	{
		for (unsigned int j = 0; j < 400; j++)
		{
			uint8_t r = (uint8_t)(((float)i / 400) * 255.f);
			uint8_t g = (uint8_t)((1.f - (float)i / 400) * 255.f);
			uint8_t b = (uint8_t)(((float)j / 400) * 255.f);
			uint32_t color = r << 16 | g << 8 | b;
			texture.setPixel(i, j, color);
		}
	}

	texture.save("test.bmp");


	std::shared_ptr<eq::Camera> camera(new eq::Camera);
	camera.get()->setPosition(eq::Math::Vector2(0, 0));
	camera.get()->setDimension(eq::Math::Vector2(800, 800));

	eq::Renderer::setClearColor(eq::Color(255, 255, 255));
	eq::Renderer::setCamera(camera);
	eq::Application::setWindowProperties(L"Test", 800, 800);
	eq::Application::setApplicationUpdate([&](float delta)
	{

		eq::Math::Vector2 mouse;
		eq::Input::getMousePosition(&mouse);

		//text.setPosition(mouse);

		if (eq::Input::isKeyPressed(EQ_W))
			camera.get()->move(eq::Math::Vector2(0, -5));
		if (eq::Input::isKeyPressed(EQ_S))
			camera.get()->move(eq::Math::Vector2(0, 5));

		if (eq::Input::isKeyPressed(EQ_A))
			camera.get()->move(eq::Math::Vector2(-5, 0));
		if (eq::Input::isKeyPressed(EQ_D))
			camera.get()->move(eq::Math::Vector2(5, 0));

		//camera.setPosition(mouse);

		//eq::Renderer::DrawCircle(camera.get()->getPosition(), 30, eq::Color(0, 0, 0));

		game.update(delta);
		game.render();

		eq::Renderer::FillCircle(mouse.x, mouse.y, 30, eq::Color(0, 0, 255, 128));
		eq::Renderer::draw(std::make_shared<eq::Text>(text));
		eq::Renderer::draw(std::make_shared<eq::Text>(text2));
		eq::Renderer::draw(std::make_shared<eq::Rectangle>(rect));
		eq::Renderer::draw(std::make_shared<eq::Ellipse>(circle));

	});

	eq::Application::start();

	return 0;
}