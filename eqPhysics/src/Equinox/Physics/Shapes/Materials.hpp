#pragma once

namespace eq
{
	namespace Physics
	{
		struct Material
		{
			float density;
			float restitution;
			float staticFriction;
			float dynamicFriction;

			Material(float density, float restitution, float staticFriction, float dynamicFriction) :
				density(density), restitution(restitution), staticFriction(staticFriction), dynamicFriction(dynamicFriction)
			{}
		};

		namespace Materials
		{
			static const Material SUPERBALL(1, 1, 0.3, 0.6);
			static const Material SANDPAPER(0.2f, 0.2, 0.6f, 0.9f);
			static const Material ICE(0.4f, 0.2f, 0.1, 0.3f);
			static const Material DEFAULT(1, 0.4, 0.3, 0.6);
			static const Material STATIC(0, 0, 0, 0);
		}

	}
}