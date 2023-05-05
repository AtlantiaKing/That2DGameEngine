#pragma once

#include "Component.h"

#include "Physics.h"
#include "Subject.h"

#include "glm/vec2.hpp"

namespace that
{
	class BoxCollider final : public Component
	{
	public:
		BoxCollider() = default;
		virtual ~BoxCollider() = default;

		BoxCollider(const BoxCollider& other) = delete;
		BoxCollider(BoxCollider&& other) = delete;
		BoxCollider& operator=(const BoxCollider& other) = delete;
		BoxCollider& operator=(BoxCollider&& other) = delete;

		virtual void Init() override;
		virtual void OnDestroy() override;

		void SetSize(const glm::vec2 size);
		void SetSize(float x, float y);
		const glm::vec2& GetSize() { return m_Size; }

		Subject<CollisionData>& OnHitEvent() { return m_OnHitEvent; }
	private:
		friend Physics;

		void Hit(BoxCollider* pOther);

		glm::vec2 m_Size{ 1.0f, 1.0f };
		bool m_DefaultSize{ true };

		Subject<CollisionData> m_OnHitEvent{};
	};
}
