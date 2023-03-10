#include "TriggerSolver.hpp"

namespace eq
{
	namespace Physics
	{
		void TriggerSolver::resolveTrigger(Manifold m)
		{
			if (m.bodyA->isTrigger()) m.bodyA->trigger();
			if (m.bodyB->isTrigger()) m.bodyB->trigger();
		}
	}
}