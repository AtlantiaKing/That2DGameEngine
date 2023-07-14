#pragma once

#include "glm/vec2.hpp"

namespace that
{
	class BoxCollider;

	struct CollisionData
	{
		BoxCollider* pCollider{};
		BoxCollider* pOther{};
		glm::vec2 contactPoint{};
		glm::vec2 fixStep{};
	};

	struct PhysicsMaterial
	{
		enum class FrictionMixer
		{
			Average,
			Multiply,
			Minimum,
			Maximum
		};

		float friction{};
		FrictionMixer frictionMixer{};
	};
}