#pragma once

#include "Component.h"
#include "Observer.h"

#include "HealthComponent.h"

namespace that
{
	class TextureComponent;
}

namespace digdug
{
	class EnemyMovement;

	class EnemyBehaviour final : public that::Component, public that::Observer<HealthComponent>
	{
	public:
		EnemyBehaviour() = default;
		virtual ~EnemyBehaviour() = default;

		EnemyBehaviour(const EnemyBehaviour& other) = delete;
		EnemyBehaviour(EnemyBehaviour&& other) = delete;
		EnemyBehaviour& operator=(const EnemyBehaviour& other) = delete;
		EnemyBehaviour& operator=(EnemyBehaviour&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void OnDestroy() override;
		virtual void Notify(const HealthComponent& health) override;
	private:
		EnemyMovement* m_pMovement{};
		HealthComponent* m_pHealth{};
		that::TextureComponent* m_pTexture{};

		const float m_TimeBetweenHeals{ 2.0f };
		float m_AccuHealTime{};
	};
}
