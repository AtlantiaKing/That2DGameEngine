#pragma once

#include "Component.h"
#include "Health.h"

#include "GridCollider.h"
#include "Observer.h"

namespace digdug
{
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
	private:
		Health m_Health{ 1 };
	};
}
