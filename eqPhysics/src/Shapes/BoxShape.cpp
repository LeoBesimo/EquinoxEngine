#include "BoxShape.hpp"

namespace eq
{
	namespace Physics
	{
		BoxShape::BoxShape(Math::Vector2 position, float angle, Material material, Math::Matrix2x2 scale) :
			Shape(position, angle, ShapeType::Box, material, scale)
		{
			m_Original[0] = Math::Vector2(-1, -1);
			m_Original[1] = Math::Vector2(1, -1);
			m_Original[2] = Math::Vector2(1, 1);
			m_Original[3] = Math::Vector2(-1, 1);
			m_Transformed.reserve(4);

			for (unsigned int i = 0; i < 4; i++)
				m_Transformed.push_back(Math::Vector2());

			transformPoints();
			calculateUnits();
			calculateBoundingBox();
		}

		void BoxShape::update(float delta)
		{
			applyGravity();
			Shape::update(delta);
			transformPoints();
		}

		void BoxShape::update(float delta, int timeSteps)
		{
			applyGravity(timeSteps);
			Shape::update(delta, timeSteps);
			transformPoints();
		}

		void BoxShape::transformPoints()
		{
			Math::Matrix2x2 rotation(getAngle());
			Math::Matrix2x2 transform = rotation * getScale();

			m_Transformed[0] = transform * m_Original[0] + getPosition();
			m_Transformed[1] = transform * m_Original[1] + getPosition();
			m_Transformed[2] = transform * m_Original[2] + getPosition();
			m_Transformed[3] = transform * m_Original[3] + getPosition();
		}

		void BoxShape::move(Math::Vector2 distance)
		{
			Shape::move(distance);
			transformPoints();
		}

		Manifold BoxShape::detectCollision(Shape* other)
		{
			return other->collideBox(this);
		}

		void BoxShape::applyGravity()
		{
			applyForce(getGravit() * getMass());

			for (uint8_t i = 0; i < 4; i++)
			{
				Math::Vector2 radius = m_Transformed[i] - getPosition();
				applyForce(getGravit() * getInertia() * 0.25, radius);
			}
		}

		void BoxShape::applyGravity(int timeSteps)
		{
			applyForce(getGravit() * getMass() / timeSteps);

			for (uint8_t i = 0; i < 4; i++)
			{
				Math::Vector2 radius = m_Transformed[i] - getPosition();
				applyForce(getGravit() * getInertia() * 0.25 / timeSteps, radius);
			}
		}

		void BoxShape::calculateBoundingBox()
		{
			float longestX = 0;
			float longestY = 0;
			Math::Vector2 position = getPosition();
			for (unsigned int i = 0; i < m_Transformed.size(); i++)
			{
				float x = abs(position.x - m_Transformed[i].x);
				float y = abs(position.y - m_Transformed[i].y);
				if (x > longestX) longestX = x;
				if (y > longestY) longestY = y;
			}

			float longest = std::max(longestX, longestY);
			Math::Vector2 min = Math::Vector2(-longest, -longest);
			Math::Vector2 max = Math::Vector2(longest, longest);
			setBoundingMin(min);
			setBoundingMax(max);
		}

		void BoxShape::calculateUnits()
		{
			Math::Matrix2x2 scale = getScale();
			float width = scale.a.x + scale.a.x;
			float height = scale.b.y + scale.b.y;
			float area = width * height;
			setMass(area * getMaterial().density);
			setInertia((1.f / 12.f) * getMass() * (width * width + height * height));
		}
	}
}