#include "../Equinox/Physics/PhysicsWorld.hpp"

namespace eq
{
	namespace Physics
	{
		void PhysicsWorld::addBody(Shape* body)
		{
			if (!std::count(m_Bodies.begin(), m_Bodies.end(), body))
			{
				m_Bodies.push_back(body);
			}
		}


		BoxShape* PhysicsWorld::addBox(Math::Vector2 position, float angle, Material material, Math::Vector2 scale, Math::Vector2 velocity, float omega, Math::Vector2 gravity)
		{
			BoxShape* box = new BoxShape(position, angle, material, Math::Matrix2x2(scale.x / 2, 0, 0, scale.y / 2));
			box->setVelocity(velocity);
			box->setOmega(omega);
			box->setGravity(M_GRAVITY);
			if (!Math::nearlyEqual(gravity, Math::Vector2())) box->setGravity(gravity);
			m_Bodies.push_back(box);
			return box;
		}

		CircleShape* PhysicsWorld::addCircle(Math::Vector2 position, float angle, float radius, Material material, Math::Vector2 velocity, float omega, Math::Vector2 gravity)
		{
			CircleShape* circle = new CircleShape(position, angle, radius, material);
			circle->setVelocity(velocity);
			circle->setOmega(omega);
			circle->setGravity(M_GRAVITY);
			if (!Math::nearlyEqual(gravity, Math::Vector2())) circle->setGravity(gravity);
			m_Bodies.push_back(circle);
			return circle;
		}

		PolygonShape* PhysicsWorld::addPolygon(Math::Vector2 position, float angle, int sides, Material material, Math::Matrix2x2 scale, Math::Vector2 velocity, float omega, Math::Vector2 gravity)
		{
			PolygonShape* polygon = new PolygonShape(position, angle, sides, material, scale * 0.5);
			polygon->setVelocity(velocity);
			polygon->setOmega(omega);
			polygon->setGravity(M_GRAVITY);
			if (!Math::nearlyEqual(gravity, Math::Vector2())) polygon->setGravity(gravity);
			m_Bodies.push_back(polygon);
			return polygon;
		}

		LineShape* PhysicsWorld::addLine(Math::Vector2 position, float angle, float length, Material material, Math::Vector2 velocity, float omega, Math::Vector2 gravity)
		{
			LineShape* line = new LineShape(position, angle, length, material);
			line->setVelocity(velocity);
			line->setOmega(omega);
			if (!Math::nearlyEqual(gravity, Math::Vector2())) line->setGravity(gravity);
			m_Bodies.push_back(line);
			return line;
		}

		LineShape* PhysicsWorld::addLine(Math::Vector2 startPosition, Math::Vector2 endPosition, Material material, Math::Vector2 velocity, float omega, Math::Vector2 gravity)
		{
			LineShape* line = new LineShape(startPosition, endPosition, material);
			line->setVelocity(velocity);
			line->setOmega(omega);
			if (!Math::nearlyEqual(gravity, Math::Vector2())) line->setGravity(gravity);
			m_Bodies.push_back(line);
			return line;
		}

		void PhysicsWorld::setWorldGravity(Math::Vector2 gravity)
		{
			this->M_GRAVITY = gravity;
			for (Shape* body : m_Bodies)
			{
				body->setGravity(M_GRAVITY);
			}
		}

		void PhysicsWorld::update(float delta)
		{
			if (delta >= 1)
				return;

			Manifold manifold;
			m_ContactPairs.clear();

			for (int i = m_Bodies.size() - 1; i >= 0; i--)
			{
				if (!inWorld(m_Bodies[i])) removeBody(i);
			}

			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				m_Bodies[i]->update(delta);
			}

			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				Shape* body = m_Bodies[i];
				for (unsigned int j = i + 1; j < m_Bodies.size(); j++)
				{
					if (i == j) continue;

					Shape* bodyB = m_Bodies[j];
					if (body->isStatic() && bodyB->isStatic()) continue;
					if (boundingBoxCollision(body, bodyB))
					{
						ContactPair pair(body, bodyB);
						m_ContactPairs.push_back(pair);
					}
				}
			}

			for (unsigned int i = 0; i < m_ContactPairs.size(); i++)
			{
				Shape* bodyA = m_ContactPairs[i].bodyA;
				Shape* bodyB = m_ContactPairs[i].bodyB;
				manifold = bodyA->detectCollision(bodyB);//m_Detector.detectCollision(bodyA, bodyB);
				if (manifold.colliding)
				{
					bodyA->getOnCollision()(manifold, bodyA);
					bodyB->getOnCollision()(manifold, bodyB);
					bodyA->setColliding(true);
					bodyB->setColliding(true);
					if (!bodyA->isTrigger() && !bodyB->isTrigger())
					{
						m_Solver.resolveDynamicWithFriction(manifold);
						m_Solver.resolveStatic(manifold);
					}
					else
					{
						m_TriggerSolver.resolveTrigger(manifold);
					}
				}
			}

			/*for (unsigned int i = 0; i < m_TimeSteps; i++)
			{
				for (unsigned int i = 0; i < m_Bodies.size(); i++)
				{
					m_Bodies[i]->update(delta, m_TimeSteps);
				}

				for (unsigned int i = 0; i < m_Bodies.size(); i++)
				{
					Shape* body = m_Bodies[i];
					for (unsigned int j = i + 1; j < m_Bodies.size(); j++)
					{
						Shape* bodyB = m_Bodies[j];
						if (body->isStatic() && bodyB->isStatic()) continue;
						if (m_Detector.boundingBoxCollision(body, bodyB))
						{
							ContactPair pair(body, bodyB);
							m_ContactPairs.push_back(pair);
						}
					}
				}

				for (unsigned int i = 0; i < m_ContactPairs.size(); i++)
				{
					Shape* bodyA = m_ContactPairs[i].bodyA;
					Shape* bodyB = m_ContactPairs[i].bodyB;
					manifold = m_Detector.detectCollision(bodyA, bodyB);
					if (manifold.colliding)
					{
						if (!bodyA->isTrigger() && !bodyB->isTrigger())
						{
							m_Solver.resolveDynamicWithFriction(manifold);
							m_Solver.resolveStatic(manifold);
						}
						else
						{
							m_TriggerSolver.resolveTrigger(manifold);
						}
					}
				}
			}*/
		}

		void PhysicsWorld::clearNonStatic()
		{
			for (int i = m_Bodies.size() - 1; i >= 0; i--)
			{
				if (!m_Bodies[i]->isStatic()) removeBody(i);
			}
		}

		void PhysicsWorld::clearStatic()
		{
			for (int i = m_Bodies.size() - 1; i >= 0; i--)
			{
				if (m_Bodies[i]->isStatic()) removeBody(i);
			}
		}

		void PhysicsWorld::clearAll()
		{
			clearNonStatic();
			clearStatic();
		}

		bool PhysicsWorld::inWorld(Shape* shape)
		{
			Math::Vector2 position = shape->getPosition();

			return !(position.x <= -m_WorldSize.x - m_WorldBorder
				|| position.x >= m_WorldSize.x + m_WorldBorder
				|| position.y <= -m_WorldSize.y - m_WorldBorder
				|| position.y >= m_WorldSize.y + m_WorldBorder);
		}

		void PhysicsWorld::removeBody(unsigned int index)
		{
			if (index >= m_Bodies.size()) return;
			delete m_Bodies[index];
			m_Bodies[index] = nullptr;
			m_Bodies.erase(m_Bodies.begin() + index);
		}
		void PhysicsWorld::removeBodyByTagID(int id)
		{
			for (int i = m_Bodies.size() - 1; i >= 0; i--)
			{
				if (m_Bodies[i]->getTag().tagId == id)
					removeBody(i);
			}
		}
		void PhysicsWorld::removeBodyByTagName(std::wstring name)
		{
			for (int i = m_Bodies.size() - 1; i >= 0; i--)
			{
				if (m_Bodies[i]->getTag().tagName == name)
					removeBody(i);
			}
		}
	}
}