#pragma once

#include "Component.h"
#include "Observer.h"

#include "Subject.h"
#include "PhysicsData.h"

namespace that
{
	class TextureMask;
	class TextureComponent;
	class BoxCollider;
}

namespace digdug
{
	class FireBreath final : public that::Component, public that::Observer<that::CollisionData>
	{
	public:
		FireBreath() = default;
		virtual ~FireBreath() = default;

		FireBreath(const FireBreath& other) = delete;
		FireBreath(FireBreath&& other) = delete;
		FireBreath& operator=(const FireBreath& other) = delete;
		FireBreath& operator=(FireBreath&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;
		virtual void Notify(const that::CollisionData& collision);
	private:
		float m_PreAttackTime{};
		float m_TimeUntilAttack{ 0.7f };

		float m_UnmaskSpeed{ 2.0f };

		that::TextureMask* m_pMask{};
		that::TextureComponent* m_pTexture{};
		that::BoxCollider* m_pCollider{};
	};
}
