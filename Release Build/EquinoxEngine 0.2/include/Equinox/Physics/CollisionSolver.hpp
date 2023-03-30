#pragma once

#include "CollisionDetector.hpp"

namespace eq
{
	namespace Physics
	{
		class CollisionSolver
		{
		public:
			CollisionSolver() {};

			void resolveStatic(Manifold m);
			void resolveDynamic(Manifold m);
			void resolveDynamicWithFriction(Manifold m);
		};
	}
}