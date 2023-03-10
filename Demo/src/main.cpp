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
	float avgFrameTime = 0;
	int avgCounter = 0;

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

	long frameCount = 0;

	eq::Physics::PhysicsWorld world(eq::Math::Vector2(4000, 4000));
	//world.addCircle(eq::Math::Vector2(-100, 0), 0, 40, eq::Physics::Materials::DEFAULT);
	world.addBox(eq::Math::Vector2(0, -200), 0, eq::Physics::Materials::STATIC, eq::Math::Vector2(600,20));
	world.addBox(eq::Math::Vector2(100, -100), 0, eq::Physics::Materials::STATIC, eq::Math::Vector2(250, 20));
	//world.addBox(eq::Math::Vector2(800, 100), eq::Math::QUARTER_PI, eq::Physics::Materials::STATIC, eq::Math::Vector2(400, 20));
	//world.addBox(eq::Math::Vector2(-800, 100), -eq::Math::QUARTER_PI, eq::Physics::Materials::STATIC, eq::Math::Vector2(400, 20));
	world.setWorldGravity(eq::Math::Vector2(0, -100));
	//eq::Physics::BoxShape* box = world.addBox(eq::Math::Vector2(0, 100), eq::Math::QUARTER_PI/2, eq::Physics::Materials::DEFAULT, eq::Math::Vector2(32, 32));
	//texture.read("test.bmp");

	//texture.save("test.bmp");

	eq::BitmapTexture wall;
	wall.read("WorldSpriteSheet.bmp");
	wall.invertY();
	eq::Sprite wallPart(wall, 48,0, 16, 16);
	wallPart.setCameraDependent(false);
	wallPart.scale(4, 4);
	wallPart.preprocessSprite();

	eq::BitmapTexture wallTexture(1424,1024);
	for (unsigned int i = 0; i < 1424; i += 64)
	{
		for (unsigned int j = 0; j < 1024; j += 64)
		{
			wallTexture.drawSprite(wallPart, i, j);
		}
	}

	eq::Sprite wallSprite(wallTexture);
	wallSprite.setCameraDependent(false);
	wallSprite.preprocessSprite();

	eq::BitmapTexture car;
	car.read("people.bmp");
	//car.setPixel(0, 0, 0xFF00FF00);
	//car.setPixel(20, 11, 0xFF0000FF);
	car.invertY();
	//car.invertX();
	//car.save("Ume.bmp");

	eq::Sprite sprite(car);// , 16, 0, 16, 16);
	sprite.scale(4,4);
	//sprite.preprocessSprite();
	//sprite.rotate(eq::Math::QUARTER_PI / 2);


	std::shared_ptr<eq::Camera> camera(new eq::Camera);
	camera.get()->setPosition(eq::Math::Vector2(400, 400));
	camera.get()->setDimension(eq::Math::Vector2(800, 800));
	eq::Ellipse cameraHighlight(camera->getPosition(), 10, eq::Color(255, 0, 255));
	cameraHighlight.setCameraDependent(false);


	wchar_t charBuffer[128];

	eq::Text frameRate;
	frameRate.setCameraDependent(false);
	eq::Text bodies;
	bodies.setCameraDependent(false);
	bodies.setPosition(eq::Math::Vector2(0, 15));
	float speed = 200;
	float zoomSpeed = 0.8;

	eq::Renderer::SetClearColor(eq::Color(255, 255, 255));
	eq::Renderer::SetCamera(camera);

	eq::Application::SetWindowProperties(L"Test", 1000, 800);
	eq::Application::SetFrameRate(60);

	eq::Application::SetApplicationInit([]() { eq::Application::SetResolution(800, 600); });

	//set application update
	eq::Application::SetApplicationUpdate([&](float delta) {

	eq::Math::Vector2 mouse;
	eq::Input::GetMousePosition(&mouse);
	mouse.x = eq::Math::map(mouse.x, 0, eq::Application::GetWindowWidth(), 0, eq::Application::GetResolutionWidth());
	mouse.y = eq::Math::map(mouse.y, 0, eq::Application::GetWindowHeight(), 0, eq::Application::GetResolutionHeight());

	//text.setPosition(mouse);

	if (eq::Input::IsKeyPressed(EQ_W))
		camera.get()->move(eq::Math::Vector2(0, speed) * delta);
	if (eq::Input::IsKeyPressed(EQ_S))
		camera.get()->move(eq::Math::Vector2(0, -speed) * delta);

	if (eq::Input::IsKeyPressed(EQ_A))
		camera.get()->move(eq::Math::Vector2(speed, 0) * delta);
	if (eq::Input::IsKeyPressed(EQ_D))
		camera.get()->move(eq::Math::Vector2(-speed, 0) * delta);

	//if (eq::Input::IsKeyPressed(EQ_I)) box->applyForce(eq::Math::Vector2(0, 300) * box->getMass());
	//sprite.move(eq::Math::Vector2(0,speed) * delta);
	//if (eq::Input::IsKeyPressed(EQ_K)) box->applyForce(eq::Math::Vector2(0, -300) * box->getMass());

	if (eq::Input::WasKeyHit(EQ_P))
	{
		eq::Application::SetFullscreen();
		//eq::Application::SetResolution(1000, 800);

	}
	if (eq::Input::WasKeyHit(EQ_O))
	{
		eq::Application::SetWindowSize(800, 600);
		//eq::Application::SetResolution(1000, 800);
	}

	if (eq::Input::WasKeyHit(EQ_K))
	{
		eq::Application::SetResolution(1920, 1080);
	}

	//sprite.move(eq::Math::Vector2(0,-speed) * delta);

	//if (eq::Input::IsKeyPressed(EQ_J)) box->applyForce(eq::Math::Vector2(-300, 0) * box->getMass());
	//sprite.move(eq::Math::Vector2(-speed,0) * delta);
	//if (eq::Input::IsKeyPressed(EQ_L)) box->applyForce(eq::Math::Vector2(300, 0) * box->getMass());
	//sprite.move(eq::Math::Vector2(speed,0) * delta);
	//camera.setPosition(mouse);

	if (eq::Input::IsKeyPressed(EQ_Q))
		camera.get()->setTransform(camera.get()->getTransform() - eq::Math::Matrix2x2(zoomSpeed, 0, 0, zoomSpeed) * delta);

	if (eq::Input::IsKeyPressed(EQ_E))
		camera.get()->setTransform(camera.get()->getTransform() + eq::Math::Matrix2x2(zoomSpeed, 0, 0, zoomSpeed) * delta);
	if (eq::Input::IsKeyPressed(EQ_F))
		camera.get()->setTransform(eq::Math::Matrix2x2(1, 0, 0, 1));

	eq::Math::Vector2 mouseTransformed = eq::Renderer::ScreenToWorldspace(mouse);

	if (eq::Input::WasMouseButtonHit(EQ_MOUSE_LEFT))
		world.addPolygon(mouseTransformed, eq::Math::QUARTER_PI, 6, eq::Physics::Materials::SUPERBALL, eq::Math::Matrix2x2(20, 0, 0, 20));

	if (eq::Input::IsKeyPressed(EQ_T))
		world.addPolygon(mouseTransformed, 0, rand() % 7 + 3, eq::Physics::Materials::DEFAULT, eq::Math::Matrix2x2(40, 0, 0, 40));
	if (eq::Input::IsKeyPressed(EQ_R))
		world.addBox(mouseTransformed, eq::Math::QUARTER_PI / 2, eq::Physics::Materials::DEFAULT, eq::Math::Vector2(40, 40));
	if (eq::Input::IsKeyPressed(EQ_G))
		for (int i = 0; i < 3; i++) world.addCircle(mouseTransformed, 0, 20, eq::Physics::Materials::DEFAULT);
	if (eq::Input::WasKeyHit(EQ_Y))
		world.clearNonStatic();


	cameraHighlight.setPosition(camera->getPosition());

	//eq::Renderer::DrawCircle(camera.get()->getPosition(), 10, eq::Color(255, 0, 255));

	//sprite.setPosition(box->getPosition() - eq::Math::Vector2(16, -32));
	//sprite.rotate(-box->getAngle());
	//sprite.rotate(frameCount / 100);
	frameCount++;
	//camera->move(eq::Math::Vector2(-box->getVelocity().x, box->getVelocity().y) * delta);
	//camera->setPosition(box->getPosition() + eq::Math::Vector2(400, 400));



	avgFrameTime += eq::Application::GetFrameTime();
	avgCounter++;
	swprintf(charBuffer, 128, L"Framerate: %f     FrameTime: %f ms    AvgFrameTime: %f ms", 1 / delta, eq::Application::GetFrameTime() * 1000, avgFrameTime / avgCounter * 1000);
	std::wstring frameText(charBuffer);
	//OutputDebugString(charBuffer);

	if (frameCount % 60 == 0)
	{
		avgCounter = 0;
		avgFrameTime = 0;
	}

	swprintf(charBuffer, 128, L"Bodies: %zd\n", world.getBodies().size());
	std::wstring bodyCount(charBuffer);
	bodies.setText(bodyCount);

	frameRate.setText(frameText);

	game.update(delta);
	world.update(delta);
	game.render();

	//eq::Renderer::FillCircle(mouse.x, mouse.y, 30, eq::Color(255, 0, 0, 160));
	//eq::Renderer::DrawSprite(sprite);
	//eq::Renderer::Draw(sprite);

	for (unsigned int x = 0; x < eq::Application::GetResolutionWidth(); x += 64)
	{
		for (unsigned int y = 0; y < eq::Application::GetResolutionHeight(); y += 64)
		{
			wallPart.setPosition(eq::Math::Vector2(x, y));
			eq::Renderer::Draw(wallPart);
		}
	}

	//eq::Renderer::Draw(wallSprite);
	//eq::Renderer::draw(std::make_shared<eq::Text>(text));
	//eq::Renderer::draw((text2));
	eq::Renderer::Draw(bodies);
	eq::Renderer::Draw(cameraHighlight);
	//eq::Renderer::draw((rect));
	//eq::Renderer::draw((circle));
	//eq::Renderer::draw((sprite));
	eq::Renderer::Draw((frameRate));
	eq::Renderer::Draw(world);
	});

	eq::Application::Start();

	return 0;
}