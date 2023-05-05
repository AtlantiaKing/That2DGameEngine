#pragma once

namespace that
{
	class BoxCollider;

	struct CollisionData
	{
		BoxCollider* pCollider;
		BoxCollider* pOther;
	};
}