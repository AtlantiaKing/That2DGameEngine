#pragma once

#include "Singleton.h"

#include <vector>

namespace that
{
	class BoxCollider;

	class Physics final : public Singleton<Physics>
	{
	public:
		Physics() = default;
		virtual ~Physics() = default;

		Physics(const Physics& other) = delete;
		Physics(Physics&& other) = delete;
		Physics& operator=(const Physics& other) = delete;
		Physics& operator=(Physics&& other) = delete;

		void AddCollider(BoxCollider* pCollider);
		void RemoveCollider(BoxCollider* pCollider);

		void ActivateDebugRendering(bool showDebug);
		bool IsShowingDebugRendering() const;

		void Update();
	private:
		bool DoOverlap(BoxCollider* pCollider, BoxCollider* pOther) const;

		std::vector<BoxCollider*> m_pColliders{};

		bool m_ShowDebug{};
	};
}

