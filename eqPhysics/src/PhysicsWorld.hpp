#pragma once

#include "CollisionDetector.hpp"
#include "CollisionSolver.hpp"
#include "Shapes/eqShapes.hpp"

namespace eq
{
	namespace Physics
	{

		struct ContactPair
		{
			Shape* bodyA;
			Shape* bodyB;

			ContactPair(Shape* a, Shape* b)
			{
				bodyA = a;
				bodyB = b;
			}
		};

		class PhysicsWorld
		{

		private:
			std::vector<Shape*> m_Bodies;
			CollisionDetector m_Detector;
			CollisionSolver m_Solver;

			std::vector<ContactPair> m_ContactPairs;

			Math::Vector2 m_WorldSize;
			float m_WorldBorder;

			Math::Vector2 M_GRAVITY;

		public:
			PhysicsWorld(Math::Vector2 worldSize) :
				m_WorldSize(worldSize)
			{
				m_WorldBorder = 150;
			}

			void addBody(Shape* body)
			{
				m_Bodies.push_back(body);
			}

			BoxShape* addBox(
				Math::Vector2 position,
				float angle,
				Material material,
				Math::Vector2 scale,
				Math::Vector2 velocity = Math::Vector2(),
				float omega = 0,
				Math::Vector2 gravity = Math::Vector2()
			);

			CircleShape* addCircle(
				Math::Vector2 position,
				float angle,
				float radius,
				Material material,
				Math::Vector2 velocity = Math::Vector2(),
				float omega = 0,
				Math::Vector2 gravity = Math::Vector2()
			);
			PolygonShape* addPolygon(
				Math::Vector2 position,
				float angle,
				int sides,
				Material material,
				Math::Matrix2x2 scale,
				Math::Vector2 velocity = Math::Vector2(),
				float omega = 0,
				Math::Vector2 gravity = Math::Vector2()
			);

			void setWorldGravity(Math::Vector2 gravity);

			std::vector<Shape*> getBodies() { return m_Bodies; }

			void update(float delta);

			void clearNonStatic();
			void clearStatic();
			void clearAll();

		private:
			bool inWorld(Shape* shape);
			void removeBody(unsigned int index);
		};
	}
}