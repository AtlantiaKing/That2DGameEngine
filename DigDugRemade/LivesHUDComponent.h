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

		virtual void Notify(const HealthComponent& pHealth) override;

		void Display(that::GameObject* pPlayer);
	private:
	};
}

