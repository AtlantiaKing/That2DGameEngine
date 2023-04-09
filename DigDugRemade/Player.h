#pragma once

#include "Component.h"
#include "Health.h"

#include "GridCollider.h"
#include "Observer.h"

#include "Event.h"

namespace that
{
	class GameObject;
}

namespace digdug
{

	class PlayerHitEvent : public that::Event
	{
	public:
		PlayerHitEvent(that::GameObject* pPlayer = nullptr) : Event{ "PlayerDeath" }, m_pPlayer{ pPlayer } {}
		that::GameObject* GetPlayer() { return m_pPlayer; }
	private:
		that::GameObject* m_pPlayer;
	};

	class Player final : public that::Component, public that::Observer<CollisionData>
	{
	public:
		Player() = default;
		virtual ~Player() = default;

		Player(const Player& other) = delete;
		Player(Player&& other) = delete;
		Player& operator=(const Player& other) = delete;
		Player& operator=(Player&& other) = delete;

		virtual void Init() override;
		virtual void Notify(const CollisionData& data) override;

		void AddScore();

		int GetHealth() { return m_Health.GetHealth(); }
		int GetScore() { return m_Score; }
	private:
		Health m_Health{ 3 };
		int m_Score{};
	};
}
