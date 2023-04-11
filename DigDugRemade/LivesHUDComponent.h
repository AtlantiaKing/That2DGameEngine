#pragma once

#include "Component.h"
#include "EventListener.h"

#include "Player.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class LivesHUDComponent final : public that::Component, public that::EventListener<PlayerHitEvent>
	{
	public:
		LivesHUDComponent() = default;
		virtual ~LivesHUDComponent() = default;

		LivesHUDComponent(const LivesHUDComponent& other) = delete;
		LivesHUDComponent(LivesHUDComponent&& other) = delete;
		LivesHUDComponent& operator=(const LivesHUDComponent& other) = delete;
		LivesHUDComponent& operator=(LivesHUDComponent&& other) = delete;

		virtual void Init() override;
		virtual void OnEvent(PlayerHitEvent*) override;

		void SetPlayer(that::GameObject* pPlayer) { m_pPlayer = pPlayer; };
	private:
		that::GameObject* m_pPlayer{};
	};
}

