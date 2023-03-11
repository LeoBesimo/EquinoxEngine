#include "CollisionSolver.hpp"
#include <Windows.h>

namespace eq
{
	namespace Physics
	{
		void CollisionSolver::resolveStatic(Manifold m)
		{
			Math::Vector2 normal = m.normal;
			m.bodyA->move(-normal * m.penetration / 2);
			m.bodyB->move(normal * m.penetration / 2);
			//m.bodyA->setPosition(m.bodyA->getPosition() - m.normal * m.penetration * m.bodyA->getInvMass());
			//m.bodyB->setPosition(m.bodyB->getPosition() + m.normal * m.penetration * m.bodyB->getInvMass());
		}
		void CollisionSolver::resolveDynamic(Manifold m)
		{
			Shape* bodyA = m.bodyA;
			Shape* bodyB = m.bodyB;

			Math::Vector2 normal = m.normal;
			Math::Vector2 contact = m.contact;

			Math::Vector2 ra = contact - bodyA->getPosition();
			Math::Vector2 rb = contact - bodyB->getPosition();

			Math::Vector2 raPerp(-ra.y, ra.x);
			Math::Vector2 rbPerp(-rb.y, rb.x);

			Math::Vector2 rv = (bodyB->getVelocity() + rbPerp * bodyB->getOmega()) - (bodyA->getVelocity() + raPerp * bodyA->getOmega());

			float contactVelMag = Math::dot(rv, normal);
			if (contactVelMag > 0)
				return;

			float e = min(bodyA->getMaterial().restitution, bodyB->getMaterial().restitution);

			float denom1 = bodyA->getInvMass() + bodyB->getInvMass();
			float denom2 = Math::square(Math::dot(raPerp, normal)) * bodyA->getInvInertia();
			float denom3 = Math::square(Math::dot(rbPerp, normal)) * bodyB->getInvInertia();

			float j = -(1 + e) * contactVelMag;
			j /= (denom1 + denom2 + denom3);

			Math::Vector2 impulse = normal * j;

			bodyA->setVelocity(bodyA->getVelocity() - impulse * bodyA->getInvMass());
			bodyB->setVelocity(bodyB->getVelocity() + impulse * bodyB->getInvMass());

			float omegaA = bodyA->getOmega() - Math::cross(ra, impulse) * bodyA->getInvInertia();
			float omegaB = bodyB->getOmega() + Math::cross(rb, impulse) * bodyB->getInvInertia();

			bodyA->setOmega(omegaA);
			bodyB->setOmega(omegaB);
		}

		void CollisionSolver::resolveDynamicWithFriction(Manifold m)
		{
			Shape* bodyA = m.bodyA;
			Shape* bodyB = m.bodyB;

			Math::Vector2 normal = m.normal;
			Math::Vector2 contact = m.contact;

			Math::Vector2 ra = contact - bodyA->getPosition();
			Math::Vector2 rb = contact - bodyB->getPosition();

			Math::Vector2 raPerp(-ra.y, ra.x);
			Math::Vector2 rbPerp(-rb.y, rb.x);

			Math::Vector2 rv = (bodyB->getVelocity() + rbPerp * bodyB->getOmega()) - (bodyA->getVelocity() + raPerp * bodyA->getOmega());

			float contactVelMag = Math::dot(rv, normal);
			if (contactVelMag > 0)
				return;

			float e = min(bodyA->getMaterial().restitution, bodyB->getMaterial().restitution);

			float denom1 = bodyA->getInvMass() + bodyB->getInvMass();
			float denom2 = Math::square(Math::dot(raPerp, normal)) * bodyA->getInvInertia();
			float denom3 = Math::square(Math::dot(rbPerp, normal)) * bodyB->getInvInertia();

			float j = -(1 + e) * contactVelMag;
			j /= (denom1 + denom2 + denom3);

			Math::Vector2 impulse = normal * j;

			bodyA->setVelocity(bodyA->getVelocity() - impulse * bodyA->getInvMass());
			bodyB->setVelocity(bodyB->getVelocity() + impulse * bodyB->getInvMass());

			float omegaA = bodyA->getOmega() - Math::cross(ra, impulse) * bodyA->getInvInertia();
			float omegaB = bodyB->getOmega() + Math::cross(rb, impulse) * bodyB->getInvInertia();

			bodyA->setOmega(omegaA);
			bodyB->setOmega(omegaB);


			//Friction
			Math::Vector2 tangent = rv - normal * contactVelMag;

			if (Math::nearlyEqual(tangent, Math::Vector2())) return;
			tangent.normalize();

			float den1 = bodyA->getInvMass() + bodyB->getInvMass();
			float den2 = Math::square(Math::dot(raPerp, tangent)) * bodyA->getInvInertia();
			float den3 = Math::square(Math::dot(rbPerp, tangent)) * bodyB->getInvInertia();

			float jt = -Math::dot(rv, tangent);
			jt /= (den1 + den2 + den3);

			Math::Vector2 frictionImpulse = tangent * jt;

			float sf = (bodyA->getMaterial().staticFriction + bodyB->getMaterial().staticFriction) / 2;//sqrt(bodyA->getMaterial().staticFriction * bodyB->getMaterial().staticFriction);
			float df = (bodyA->getMaterial().dynamicFriction + bodyB->getMaterial().dynamicFriction) / 2;//sqrt(bodyA->getMaterial().dynamicFriction * bodyB->getMaterial().dynamicFriction);

			if (abs(jt) < j * sf)
				frictionImpulse = tangent * jt;
			else
				frictionImpulse = tangent * -j * df;

			bodyA->setVelocity(bodyA->getVelocity() - frictionImpulse * bodyA->getInvMass());
			bodyB->setVelocity(bodyB->getVelocity() + frictionImpulse * bodyB->getInvMass());

			float omegaAT = bodyA->getOmega() - Math::cross(ra, frictionImpulse) * bodyA->getInvInertia();
			float omegaBT = bodyB->getOmega() + Math::cross(rb, frictionImpulse) * bodyB->getInvInertia();

			bodyA->setOmega(omegaAT);
			bodyB->setOmega(omegaBT);
		}
	}
}