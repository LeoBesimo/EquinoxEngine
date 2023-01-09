#include "Equinox.hpp"
#include "Testgame.hpp"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

equinoxAppEntryPoint
{
	srand(time(NULL));

	Testgame game;

	std::wstring t(L"Test");

	eq::Text text(eq::Math::Vector2(0, 0), t);
	text.setCameraDependent(true);

	eq::Text text2(eq::Math::Vector2(100, 100), std::wstring(L"yeet"));

	eq::Rectangle rect(eq::Math::Vector2(200, 200), eq::Math::Vector2(100, 100), eq::Color(255,0,0));
	rect.setCameraDependent(true);

	eq::Ellipse circle(eq::Math::Vector2(0, -100), 30, eq::Color(0, 255, 0, 128));
	circle.setCameraDependent(true);

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

	float frameCount = 0;

	eq::Physics::PhysicsWorld world(eq::Math::Vector2(4000, 4000));
	world.addCircle(eq::Math::Vector2(-100, 0), 0, 40, eq::Physics::Materials::DEFAULT);
	world.addBox(eq::Math::Vector2(0, -200), 0, eq::Physics::Materials::STATIC, eq::Math::Vector2(600,20));
	//world.addBox(eq::Math::Vector2(800, 100), eq::Math::QUARTER_PI, eq::Physics::Materials::STATIC, eq::Math::Vector2(400, 20));
	//world.addBox(eq::Math::Vector2(-800, 100), -eq::Math::QUARTER_PI, eq::Physics::Materials::STATIC, eq::Math::Vector2(400, 20));
	world.setWorldGravity(eq::Math::Vector2(0, -100));
	eq::Physics::BoxShape* box = world.addBox(eq::Math::Vector2(0, 100), 0, eq::Physics::Materials::DEFAULT, eq::Math::Vector2(32, 32));
	//texture.read("test.bmp");

	//texture.save("test.bmp");

	eq::BitmapTexture car;
	car.read("people.bmp");
	//car.setPixel(0, 0, 0xFF00FF00);
	//car.setPixel(20, 11, 0xFF0000FF);
	car.invertY();
	//car.invertX();
	//car.save("Ume.bmp");

	eq::Sprite sprite(car,16,16);// , 16, 0, 16, 16);
	sprite.scale(4,4);
	//sprite.rotate(eq::Math::QUARTER_PI / 2);


	std::shared_ptr<eq::Camera> camera(new eq::Camera);
	camera.get()->setPosition(eq::Math::Vector2(400, 400));
	camera.get()->setDimension(eq::Math::Vector2(800, 800));

	wchar_t charBuffer[128];

	eq::Text frameRate;
	frameRate.setCameraDependent(false);
	eq::Text bodies;
	bodies.setCameraDependent(false);
	bodies.setPosition(eq::Math::Vector2(0, 15));
	float speed = 200;
	float zoomSpeed = 0.8;

	eq::Renderer::setClearColor(eq::Color(255, 255, 255));
	eq::Renderer::setCamera(camera);

	eq::Application::SetWindowProperties(L"Test", 1400, 1400);

	//set application update
	eq::Application::SetApplicationUpdate([&](float delta) {

		eq::Math::Vector2 mouse;
	eq::Input::getMousePosition(&mouse);

	//text.setPosition(mouse);

	if (eq::Input::isKeyPressed(EQ_W))
		camera.get()->move(eq::Math::Vector2(0, speed) * delta);
	if (eq::Input::isKeyPressed(EQ_S))
		camera.get()->move(eq::Math::Vector2(0, -speed) * delta);

	if (eq::Input::isKeyPressed(EQ_A))
		camera.get()->move(eq::Math::Vector2(speed, 0) * delta);
	if (eq::Input::isKeyPressed(EQ_D))
		camera.get()->move(eq::Math::Vector2(-speed, 0) * delta);

	if (eq::Input::isKeyPressed(EQ_I))
		box->applyForce(eq::Math::Vector2(0, 300) * box->getMass());
	//sprite.move(eq::Math::Vector2(0,speed) * delta);
	if (eq::Input::isKeyPressed(EQ_K))
		box->applyForce(eq::Math::Vector2(0, -300) * box->getMass());

	//sprite.move(eq::Math::Vector2(0,-speed) * delta);

	if (eq::Input::isKeyPressed(EQ_J))
		box->applyForce(eq::Math::Vector2(-300, 0) * box->getMass());
	//sprite.move(eq::Math::Vector2(-speed,0) * delta);
	if (eq::Input::isKeyPressed(EQ_L))
		box->applyForce(eq::Math::Vector2(300, 0) * box->getMass());
	//sprite.move(eq::Math::Vector2(speed,0) * delta);
	//camera.setPosition(mouse);

	if (eq::Input::isKeyPressed(EQ_Q))
		camera.get()->setTransform(camera.get()->getTransform() - eq::Math::Matrix2x2(zoomSpeed, 0, 0, zoomSpeed) * delta);

	if (eq::Input::isKeyPressed(EQ_E))
		camera.get()->setTransform(camera.get()->getTransform() + eq::Math::Matrix2x2(zoomSpeed, 0, 0, zoomSpeed) * delta);
	if (eq::Input::isKeyPressed(EQ_F))
		camera.get()->setTransform(eq::Math::Matrix2x2(1, 0, 0, 1));

	eq::Math::Vector2 mouseTransformed = eq::Renderer::ScreenToWorldspace(mouse);

	if (eq::Input::wasMouseButtonHit(EQ_MOUSE_LEFT))
		world.addPolygon(mouseTransformed, 0, 5, eq::Physics::Materials::DEFAULT, eq::Math::Matrix2x2(20, 0, 0, 20));

	if (eq::Input::isKeyPressed(EQ_T))
		world.addPolygon(mouseTransformed, 0, rand() % 7 + 3, eq::Physics::Materials::DEFAULT, eq::Math::Matrix2x2(40, 0, 0, 40));
	if (eq::Input::isKeyPressed(EQ_R))
		world.addBox(mouseTransformed, eq::Math::QUARTER_PI / 2, eq::Physics::Materials::DEFAULT, eq::Math::Vector2(40, 40));
	if (eq::Input::isKeyPressed(EQ_G))
		for(int i = 0; i < 3; i++) world.addCircle(mouseTransformed, 0, 20, eq::Physics::Materials::DEFAULT);
	if (eq::Input::wasKeyHit(EQ_Y))
		world.clearNonStatic();

	eq::Renderer::DrawCircle(camera.get()->getPosition(), 10, eq::Color(255, 0, 255));

	//sprite.setPosition(box->getPosition() - eq::Math::Vector2(16, -32));
	//sprite.rotate(-box->getAngle());
	sprite.rotate(frameCount / 100);
	frameCount++;
	//camera->move(eq::Math::Vector2(-box->getVelocity().x, box->getVelocity().y) * delta);
	//camera->setPosition(box->getPosition() + eq::Math::Vector2(400, 400));

	swprintf(charBuffer, 128, L"Framerate: %f     FrameTime: %f ms", 1 / delta, eq::Application::getFrameTime() * 1000);
	std::wstring frameText(charBuffer);
	//OutputDebugString(charBuffer);

	swprintf(charBuffer, 128, L"Bodies: %d\n", world.getBodies().size());
	std::wstring bodyCount(charBuffer);
	bodies.setText(bodyCount);

	frameRate.setText(frameText);

	game.update(delta);
	world.update(delta);
	game.render();

	//eq::Renderer::FillCircle(mouse.x, mouse.y, 30, eq::Color(255, 0, 0, 160));
	eq::Renderer::DrawSprite(sprite);
	//eq::Renderer::draw(std::make_shared<eq::Text>(text));
	//eq::Renderer::draw((text2));
	eq::Renderer::draw(bodies);
	//eq::Renderer::draw((rect));
	//eq::Renderer::draw((circle));
	//eq::Renderer::draw((sprite));
	eq::Renderer::draw((frameRate));
	eq::Renderer::draw(world);
	});

	eq::Application::Start();

	return 0;
}