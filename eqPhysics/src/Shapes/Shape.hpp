#pragma once
#include <vector>


#include <eqMath.hpp>
#include "Materials.hpp"


namespace eq
{
	namespace Physics
	{
		enum class ShapeType
		{
			Box, Polygon, Circle
		};

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

		public:
			Shape(Math::Vector2 position, float angle, ShapeType type, Material material, Math::Matrix2x2 scale);

			virtual void update(float delta);

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

			bool canRotate() { return m_InvInertia != 0; }
			bool canMove() { return m_InvMass != 0; }
			bool isStatic() { return !canMove() && !canRotate(); }

			virtual void move(Math::Vector2 distance);

		private:
			virtual void applyGravity() = 0;
			virtual void calculateBoundingBox() = 0;
			void integrateForces(float delta);
			void integrateVelocities(float delta);

			virtual void calculateUnits() = 0;
		};
	}
}