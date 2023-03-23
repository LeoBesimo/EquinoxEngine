#include "../../Equinox/Physics/Shapes/Shape.hpp"


namespace eq
{
	namespace Physics
	{
		Shape::Shape(Math::Vector2 position, float angle, ShapeType type, Material material, Math::Matrix2x2 scale) :
			m_Position(position), m_Angle(angle), m_Type(type), m_Material(material), m_Scale(scale), m_Color(0xFF000000)
		{
			m_Velocity = Math::Vector2();
			m_Force = Math::Vector2();
			m_Omega = 0;
			m_Torque = 0;
			m_IsTrigger = false;
			m_Trigger = [&]() {};
			m_OnCollision = [&](Manifold m, Shape* self) {};
		}

		void Shape::update(float delta)
		{
			m_IsColliding = false;
			integrateForces(delta);
			integrateVelocities(delta);
		}

		void Shape::update(float delta, int timeSteps)
		{
			integrateForces(delta);
			integrateVelocities(delta, timeSteps);
		}

		void Shape::applyForce(Math::Vector2 force)
		{
			this->m_Force += force;
		}

		void Shape::applyForce(Math::Vector2 force, Math::Vector2 radius)
		{
			m_Torque += Math::cross(force, radius);
		}

		void Shape::setMass(float mass)
		{
			this->m_Mass = mass;
			this->m_InvMass = mass == 0 ? 0 : 1 / mass;
		}

		void Shape::setInertia(float inertia)
		{
			this->m_Inertia = inertia;
			this->m_InvInertia = inertia == 0 ? 0 : 1 / inertia;
		}

		void Shape::setStatic()
		{
			setMass(0);
			setInertia(0);
		}

		void Shape::move(Math::Vector2 distance)
		{
			if (m_InvMass == 0) return;
			this->m_Position += distance;
		}

		void Shape::integrateForces(float delta)
		{
			m_Velocity += m_Force * m_InvMass * delta;
			m_Omega += (m_Torque * m_InvInertia * delta);
			m_Force *= 0;
			m_Torque = 0;
		}

		void Shape::integrateVelocities(float delta)
		{
			m_Position += m_Velocity * delta;
			m_Angle += m_Omega * delta;
		}
		void Shape::integrateVelocities(float delta, int timeSteps)
		{
			m_Position += m_Velocity * delta / timeSteps;
			m_Angle += m_Omega * delta / timeSteps;
		}
	}
}