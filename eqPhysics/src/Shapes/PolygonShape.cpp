#include "PolygonShape.hpp"

namespace eq
{
	namespace Physics
	{
		PolygonShape::PolygonShape(Math::Vector2 position, float angle, float sides, Material material, Math::Matrix2x2 scale) :
			Shape(position, angle, ShapeType::Polygon, material, scale), m_Sides(sides)
		{
			Math::Matrix2x2 transform = getScale() * Math::Matrix2x2(getAngle());
			float ac = Math::TWO_PI / sides;
			float maxAngle = sides <= 6 ? Math::TWO_PI : Math::TWO_PI - ac;
			int count = 0;
			for (float i = 0; i < maxAngle && count < sides; i += ac)
			{
				Math::Vector2 originalPoint(cos(i), sin(i));
				m_Original.push_back(originalPoint);
				m_Transformed.push_back(transform * originalPoint + getPosition());
				count++;
			}
			calculateUnits();
			calculateBoundingBox();
		}

		void PolygonShape::update(float delta)
		{
			applyGravity();
			Shape::update(delta);
			transformPoints();
		}

		void PolygonShape::transformPoints()
		{
			Math::Matrix2x2 rotation = Math::Matrix2x2(getAngle());
			for (unsigned int i = 0; i < m_Original.size(); i++)
			{
				Math::Matrix2x2 transform = rotation * getScale();
				m_Transformed[i] = transform * m_Original[i] + getPosition();
			}
		}

		void PolygonShape::move(Math::Vector2 distance)
		{
			Shape::move(distance);
			transformPoints();
		}

		void PolygonShape::applyGravity()
		{
			applyForce(getGravit() * getMass());

			float factor = 1 / m_Transformed.size();

			for (unsigned int i = 0; i < m_Transformed.size(); i++)
			{
				Math::Vector2 radius = m_Transformed[i] - getPosition();
				applyForce(getGravit() * getInertia() * factor, radius);
			}
		}

		void PolygonShape::calculateBoundingBox()
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

		void PolygonShape::calculateUnits()
		{
			int n = m_Transformed.size();
			Math::Vector2 center = getPosition();

			Material material = getMaterial();

			float totalMass = 0;
			float totalInertia = 0;

			for (unsigned int i = 0; i < n; i++)
			{
				Math::Vector2 p1 = m_Transformed[i];
				Math::Vector2 p2 = m_Transformed[(i + 1) % n];

				Math::Vector2 midpoint;
				float heightSqr = Math::distPointToLine(center, p1, p2, &midpoint);
				float height = sqrt(heightSqr);

				Math::Vector2 triangleCenter1 = (center + p1 + midpoint) / 3;
				Math::Vector2 triangleCenter2 = (center + p2 + midpoint) / 3;

				float b1 = (p1 - midpoint).len();
				float b2 = (p2 - midpoint).len();

				float area1 = height * b1 / 2;
				float area2 = height * b2 / 2;

				float mass1 = area1 * material.density;
				float mass2 = area2 * material.density;

				float j1 = (mass1 / 18) * (b1 + heightSqr);
				float j2 = (mass2 / 18) * (b2 + heightSqr);

				totalMass += mass1;
				totalMass += mass2;

				totalInertia += j1 * (triangleCenter1 - center).len();//Math::pythagoreanSolve(center, triangleCenter1);
				totalInertia += j2 * (triangleCenter2 - center).len();//Math::pythagoreanSolve(center, triangleCenter2);
			}

			setMass(totalMass);
			setInertia(totalInertia);
		}
	}
}