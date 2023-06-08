#pragma once

#include "Component.h"
#include "Observer.h"
#include "HealthComponent.h"

namespace digdug
{
	class LivesHUDComponent final : public that::Component, public that::Observer<HealthComponent>
	{
	public:
		LivesHUDComponent() = default;
		virtual ~LivesHUDComponent() = default;

		LivesHUDComponent(const LivesHUDComponent& other) = delete;
		LivesHUDComponent(LivesHUDComponent&& other) = delete;
		LivesHUDComponent& operator=(const LivesHUDComponent& other) = delete;
		LivesHUDComponent& operator=(LivesHUDComponent&& other) = delete;

		virtual void Update() override;
		virtual void Notify(const HealthComponent& health) override;
		virtual void OnDestroy() override;
		virtual void OnSubjectDestroy() override;

		void SearchPlayer(that::GameObject* pPlayerParent);

	private:
		void Display(that::GameObject* pPlayer);

		that::GameObject* m_pParent{};
		digdug::HealthComponent* m_pHealth{};
	};
}

