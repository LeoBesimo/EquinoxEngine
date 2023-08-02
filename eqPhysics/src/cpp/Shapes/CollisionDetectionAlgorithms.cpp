#include "../../Equinox/Physics/Shapes/CollisionDetectionAlgorithms.hpp"

#include "../../Equinox/Physics/Shapes/LineShape.hpp"
#include "../../Equinox/Physics/Shapes/PolygonShape.hpp"
#include "../../Equinox/Physics/Shapes/BoxShape.hpp"
#include "../../Equinox/Physics/Shapes/CircleShape.hpp"

namespace eq {
	namespace Physics
	{
		Manifold BoxBoxCollision(Shape* bodyA, Shape* bodyB)
		{
			return Manifold();
		}

		Manifold CircleBoxCollison(Shape* bodyA, Shape* bodyB)
		{
			return Manifold();
		}

		Manifold BoxPolygonCollision(Shape* a, Shape* b)
		{
			Manifold m;

			BoxShape* bodyA = static_cast<BoxShape*>(a);
			PolygonShape* bodyB = static_cast<PolygonShape*>(b);


			m.bodyA = a;
			m.bodyB = b;

			std::vector<Math::Vector2> normalsPoly1 = getNormals(bodyA->getCorners());
			std::vector<Math::Vector2> normalsPoly2 = getNormals(bodyB->getCorners());

			bool separated = false;

			Math::Vector2 normalA;
			Math::Vector2 normalB;
			Math::Vector2 normal;
			float minDepthA = FLT_MAX;
			float minDepthB = FLT_MAX;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly1.size(); i++)
			{
				Math::Vector2 projectionA = getMinMax(bodyA->getCorners(), normalsPoly1[i]);
				Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), normalsPoly1[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
				if (separated) break;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepthA)
				{
					minDepthA = depth;
					normalA = normalsPoly1[i];
				}
			}

			if (!separated)
			{
				for (unsigned int i = 0; i < normalsPoly2.size(); i++)
				{
					Math::Vector2 projectionA = getMinMax(bodyA->getCorners(), normalsPoly2[i]);
					Math::Vector2 projectionB = getMinMax(bodyB->getCorners(), normalsPoly2[i]);


					separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
					if (separated) break;

					float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

					if (depth < minDepthB)
					{
						minDepthB = depth;
						normalB = normalsPoly2[i];
					}
				}
			}

			m.colliding = !separated;

			if (!separated)
			{
				float penetrationA = minDepthA / normalA.len();
				float penetrationB = minDepthB / normalB.len();

				if (penetrationA < penetrationB)
				{
					m.penetration = penetrationA;
					normal = normalA;
				}
				else
				{
					m.penetration = penetrationB;
					normal = normalB;
				}

				Math::Vector2 ab = bodyB->getPosition() - bodyA->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				//m.penetration = minDepth / normal.len();

				//Math::Vector2 penetration = normal.normalize() * minDepth;

				//m.penetration = penetration.len();

				m.normal = normal.normalize();
				m.contact = getContactBoxPolygon(bodyA, bodyB);
			}

			return m;
		}
		
		Manifold LineBoxCollision(Shape* a, Shape* b)
		{
			
			LineShape* line = static_cast<LineShape*>(b);
			BoxShape* box = static_cast<BoxShape*>(a);

			Manifold m;
			m.bodyA = a;
			m.bodyB = b;

			std::vector<Math::Vector2> normalsPoly = getNormals(box->getCorners());
			Math::Vector2 lineVector = line->getEndPosition() - line->getStartPosition();
			Math::Vector2 normalLine = Math::Vector2(-lineVector.y, lineVector.x);

			bool separated = false;

			Math::Vector2 normal;
			float minDepth = FLT_MAX;

			for (unsigned int i = 0; i < normalsPoly.size(); i++)
			{
				Math::Vector2 projectionA = getMinMax(box->getCorners(), normalsPoly[i]);
				float dotLineA = Math::dot(line->getStartPosition(), normalsPoly[i]);
				float dotLineB = Math::dot(line->getEndPosition(), normalsPoly[i]);
				Math::Vector2 projectionB = Math::Vector2(std::min(dotLineA, dotLineB), std::max(dotLineA, dotLineB));//getMinMax(bodyB->getCorners(), normalsPoly1[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;
				if (separated) break;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepth)
				{
					minDepth = depth;
					normal = normalsPoly[i];
				}
			}

			if (!separated)
			{
				Math::Vector2 projectionA = getMinMax(box->getCorners(), normalLine);
				float dotLineA = Math::dot(line->getStartPosition(), normalLine);
				float dotLineB = Math::dot(line->getEndPosition(), normalLine);
				Math::Vector2 projectionB = Math::Vector2(std::min(dotLineA, dotLineB), std::max(dotLineA, dotLineB));//getMinMax(bodyB->getCorners(), normalsPoly1[i]);

				separated = projectionA.x >= projectionB.y || projectionB.x >= projectionA.y;

				float depth = std::min(projectionB.y - projectionA.x, projectionA.y - projectionB.x);

				if (depth < minDepth)
				{
					minDepth = depth;
					normal = normalLine;
				}
			}

			m.colliding = !separated;

			if (!separated)
			{
				Math::Vector2 ab = line->getPosition() - box->getPosition();

				if (Math::dot(ab, normal) < 0) normal *= -1;

				m.penetration = minDepth / normal.len();
				m.normal = normal.normalize();
				m.contact = getContactLineBox(line, box);
			}

			return m;
		}
	}
}
