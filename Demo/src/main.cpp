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

	int w = 200;
	int h = 200;

	eq::BitmapTexture texture(w,h);
	for (unsigned int i = 0; i < w; i++)
	{
		for (unsigned int j = 0; j < h; j++)
		{
			uint8_t r = (uint8_t)(((float)i / w) * 255.f);
			uint8_t g = (uint8_t)((1.f - (float)i / w) * 255.f);
			uint8_t b = (uint8_t)(((float)j / h) * 255.f);
			uint32_t color = 255 << 24 | r << 16 | g << 8 | b;
			texture.setPixel(i, j, color);
		}
	}

	//texture.read("test.bmp");

	//texture.save("test.bmp");

	eq::BitmapTexture car;
	car.read("people.bmp");
	//car.setPixel(0, 0, 0xFF00FF00);
	//car.setPixel(20, 11, 0xFF0000FF);
	car.invertY();
	//car.invertX();
	//car.save("Ume.bmp");

	eq::Sprite sprite(car,16,0,16,16);
	sprite.scale(4, 4);
	sprite.setAlpha(255);


	std::shared_ptr<eq::Camera> camera(new eq::Camera);
	camera.get()->setPosition(eq::Math::Vector2(0, 0));
	camera.get()->setDimension(eq::Math::Vector2(800, 800));

	wchar_t charBuffer[128];

	eq::Text frameRate;
	frameRate.setCameraDependent(false);

	float speed = 200;

	eq::Renderer::setClearColor(eq::Color(255, 255, 255));
	eq::Renderer::setCamera(camera);
	eq::Application::setWindowProperties(L"Test", 800, 800);
	eq::Application::setApplicationUpdate([&](float delta)
	{

		eq::Math::Vector2 mouse;
		eq::Input::getMousePosition(&mouse);

		//text.setPosition(mouse);

		if (eq::Input::isKeyPressed(EQ_W))
			camera.get()->move(eq::Math::Vector2(0, -speed) * delta);
		if (eq::Input::isKeyPressed(EQ_S))
			camera.get()->move(eq::Math::Vector2(0, speed) * delta);

		if (eq::Input::isKeyPressed(EQ_A))
			camera.get()->move(eq::Math::Vector2(-speed, 0) * delta);
		if (eq::Input::isKeyPressed(EQ_D))
			camera.get()->move(eq::Math::Vector2(speed, 0) * delta);

		if (eq::Input::isKeyPressed(EQ_I))
			sprite.move(eq::Math::Vector2(0,-speed) * delta);
		if (eq::Input::isKeyPressed(EQ_K))
			sprite.move(eq::Math::Vector2(0,speed) * delta);

		if (eq::Input::isKeyPressed(EQ_J))
			sprite.move(eq::Math::Vector2(-speed,0) * delta);
		if (eq::Input::isKeyPressed(EQ_L))
			sprite.move(eq::Math::Vector2(speed,0) * delta);
		//camera.setPosition(mouse);

		//eq::Renderer::DrawCircle(camera.get()->getPosition(), 30, eq::Color(0, 0, 0));

		swprintf(charBuffer, 128, L"Framerate %f\n", 1 / delta);
		std::wstring frameText(charBuffer);
		//OutputDebugString(charBuffer);
		
		frameRate.setText(frameText);

		game.update(delta);
		game.render();

		eq::Renderer::FillCircle(mouse.x, mouse.y, 30, eq::Color(255, 0, 0, 160));
		//eq::Renderer::DrawSprite(sprite);
		//eq::Renderer::draw(std::make_shared<eq::Text>(text));
		eq::Renderer::draw(std::make_shared<eq::Text>(text2));
		eq::Renderer::draw(std::make_shared<eq::Rectangle>(rect));
		eq::Renderer::draw(std::make_shared<eq::Ellipse>(circle));
		eq::Renderer::draw(std::make_shared<eq::Sprite>(sprite));
		eq::Renderer::draw(std::make_shared<eq::Text>(frameRate));

	});

	eq::Application::start();

	return 0;
}