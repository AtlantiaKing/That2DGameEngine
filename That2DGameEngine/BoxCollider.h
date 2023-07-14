#pragma once

#include "Component.h"

#include "PhysicsData.h"
#include "Physics.h"
#include "Subject.h"

#include "glm/vec2.hpp"

#include <set>

namespace that
{
	class BoxCollider final : public Component
	{
	public:
		BoxCollider();
		virtual ~BoxCollider() = default;

		BoxCollider(const BoxCollider& other) = delete;
		BoxCollider(BoxCollider&& other) = delete;
		BoxCollider& operator=(const BoxCollider& other) = delete;
		BoxCollider& operator=(BoxCollider&& other) = delete;

		virtual void Init() override;
		virtual void OnDestroy() override;
		virtual void LateUpdate() override;
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
		void SetIgnoreGroup(unsigned int ignoreGroup) { m_IgnoreGroup = ignoreGroup; }
		unsigned int GetIgnoreGroup() const { return m_IgnoreGroup; }

		const PhysicsMaterial& GetMaterial() const { return m_Material; }

		Subject<CollisionData> OnCollisionEnter{};
		Subject<CollisionData> OnCollision{};
		Subject<CollisionData> OnCollisionExit{};

	private:
		friend Physics;

		void Hit(const CollisionData& pOther);

		glm::vec2 m_Center{};
		glm::vec2 m_Size{ 1.0f, 1.0f };
		bool m_DefaultSize{ true };

		unsigned int m_Layer{};
		unsigned int m_IgnoreGroup{};

		PhysicsMaterial m_Material{};
		static PhysicsMaterial m_DefaultMaterial;

		std::set<BoxCollider*> m_CollisionsFrame{};
		std::set<BoxCollider*> m_Collisions{};
	};
}
