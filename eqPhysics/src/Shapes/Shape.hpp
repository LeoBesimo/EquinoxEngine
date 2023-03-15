#pragma once
#include <vector>
#include <functional>

#include <eqMath.hpp>
#include "Materials.hpp"
#include "CollisionUtilities.hpp"


namespace eq
{
	namespace Physics
	{
		enum class ShapeType
		{
			Box, Polygon, Circle, Line
		};

		struct Manifold;

		class Shape
		{
		private:

			ShapeType m_Type;
			Material m_Material;

			Math::Vector2 m_BoundingBoxMin;
			Math::Vector2 m_BoundingBoxMax;

			Math::Vector2 m_Position;
			Math::Vector2 m_Velocity;
			Math::Vector2 m_Force;

			Math::Vector2 m_Gravity;

			float m_Angle;
			float m_Omega;
			float m_Torque;

			Math::Matrix2x2 m_Scale;

			float m_Mass;
			float m_Inertia;
			float m_InvMass;
			float m_InvInertia;

			uint32_t m_Color;

			uint32_t m_IsTrigger;

			std::function<void()> m_Trigger;
			std::function<void(Manifold m, Shape* self)> m_OnCollision;

		public:
			Shape(Math::Vector2 position, float angle, ShapeType type, Material material, Math::Matrix2x2 scale);

			virtual void update(float delta);
			virtual void update(float delta, int timeSteps);

			void applyForce(Math::Vector2 force);
			void applyForce(Math::Vector2 force, Math::Vector2 radius);

		public:
			//Get
			ShapeType getShapeType() { return m_Type; }
			Material getMaterial() { return m_Material; }

			Math::Vector2 getPosition() { return m_Position; }
			Math::Vector2 getVelocity() { return m_Velocity; }
			Math::Vector2 getForce() { return m_Force; }

			Math::Vector2 getGravit() { return m_Gravity; }

			float getAngle() { return m_Angle; }
			float getOmega() { return m_Omega; }
			float getTorque() { return m_Torque; }

			Math::Matrix2x2 getScale() { return m_Scale; }

			float getMass() { return m_Mass; }
			float getInertia() { return m_Inertia; }
			float getInvMass() { return m_InvMass; }
			float getInvInertia() { return m_InvInertia; }

			Math::Vector2 getBoundingMin() { return m_BoundingBoxMin; }
			Math::Vector2 getBoundingMax() { return m_BoundingBoxMax; }

			//Set
			void setShapeType(ShapeType type) { this->m_Type = type; }
			void setMaterial(Material material) { this->m_Material = material; }

			void setPosition(Math::Vector2 position) { this->m_Position = position; }
			void setVelocity(Math::Vector2 velocity) { this->m_Velocity = velocity; }
			void setForce(Math::Vector2 force) { this->m_Force = force; }

			void setGravity(Math::Vector2 gravity) { this->m_Gravity = gravity; }

			void setAngle(float angle) { this->m_Angle = angle; }
			void setOmega(float omega) { this->m_Omega = omega; }
			void setTorque(float torque) { this->m_Torque = torque; }

			void setScale(Math::Matrix2x2 scale) { this->m_Scale = scale; }

			void setBoundingMin(Math::Vector2 min) { this->m_BoundingBoxMin = min; }
			void setBoundingMax(Math::Vector2 max) { this->m_BoundingBoxMax = max; }

			void setMass(float mass);
			void setInertia(float inertia);

			void setStatic();

			void setColor(const uint32_t& color) { m_Color = color; }
			uint32_t& getColor() { return m_Color; }

			void setTrigger(bool isTrigger) { this->m_IsTrigger = isTrigger; }
			void setTriggerFunction(const std::function<void()>& func) { this->m_Trigger = func; }
			void setOnCollisionFunction(const std::function<void(Manifold m, Shape* self)>& func) { this->m_OnCollision = func; }

			void trigger() { this->m_Trigger(); }
			std::function<void()>& getTrigger() { return this->m_Trigger; }
			std::function<void(Manifold m, Shape* self)>& getOnCollision() { return this->m_OnCollision; }

			bool canRotate() { return m_InvInertia != 0; }
			bool canMove() { return m_InvMass != 0; }
			bool isStatic() { return !canMove() && !canRotate(); }
			bool isTrigger() { return m_IsTrigger; }

			virtual void move(Math::Vector2 distance);

			virtual Manifold detectCollision(Shape* other) = 0;
			virtual Manifold collideLine(Shape* other) = 0;
			virtual Manifold collideCircle(Shape* other) = 0;
			virtual Manifold collidePolygon(Shape* other) = 0;
			virtual Manifold collideBox(Shape* other) = 0;

		private:
			virtual void applyGravity() = 0;
			virtual void applyGravity(int timeSteps) = 0;
			virtual void calculateBoundingBox() = 0;
			void integrateForces(float delta);
			void integrateVelocities(float delta);
			void integrateVelocities(float delta, int timeSteps);

			virtual void calculateUnits() = 0;
		};

		struct Manifold
		{
			bool colliding;
			Shape* bodyA;
			Shape* bodyB;
			Math::Vector2 normal;
			float penetration;
			Math::Vector2 contact;
		};
	}
}