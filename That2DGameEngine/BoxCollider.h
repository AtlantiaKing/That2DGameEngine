#pragma once

#include "Component.h"

#include "PhysicsData.h"
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
		virtual void Render() const override;

		void SetCenter(const glm::vec2& center);
		void SetCenter(float x, float y);
		const glm::vec2& GetCenter() const { return m_Center; }
		glm::vec2 GetCenterWorld() const;
		void SetSize(const glm::vec2& size);
		void SetSize(float x, float y);
		const glm::vec2& GetSize() const { return m_Size; }
		glm::vec2 GetSizeWorld() const;

		void SetLayer(unsigned int layer) { m_Layer = layer; }
		unsigned int GetLayer() const { return m_Layer; }

		Subject<CollisionData>& OnHitEvent() { return m_OnHitEvent; }
	private:
		friend Physics;

		void Hit(BoxCollider* pOther);

		glm::vec2 m_Center{};
		glm::vec2 m_Size{ 1.0f, 1.0f };
		bool m_DefaultSize{ true };

		unsigned int m_Layer{};

		Subject<CollisionData> m_OnHitEvent{};
	};
}
