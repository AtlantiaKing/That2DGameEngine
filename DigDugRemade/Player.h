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
	struct PlayerHitEvent final : public that::Event
	{
		PlayerHitEvent(that::GameObject* _pPlayer = nullptr) : Event{ "PlayerDeath" }, pPlayer{ _pPlayer } {}
		virtual ~PlayerHitEvent() = default;

		that::GameObject* pPlayer;
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
		int GetScore() { return m_Score; }
	private:
		int m_Score{};
	};
}
