#pragma once

#include "CollisionDetector.hpp"

namespace eq
{
	namespace Physics
	{
		class TriggerSolver
		{
		public:
			TriggerSolver() {};

			void resolveTrigger(Manifold m);
		};
	}
}