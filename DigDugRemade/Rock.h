#pragma once

#include "Component.h"

#include "State.h"

#include <vector>

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GridComponent;

	class Rock final : public that::Component
	{
	public:
		Rock() = default;
		~Rock() = default;

		Rock(const Rock& other) = delete;
		Rock(Rock&& other) = delete;
		Rock& operator=(const Rock& other) = delete;
		Rock& operator=(Rock&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;

		void Start(const std::vector<that::GameObject*>& pPlayers);
		that::GameObject* GetDiggingPlayer();
		that::GameObject* GetLastPlayer() const;
	private:
		void ChangeState(std::unique_ptr<digdug::State> pState);

		std::vector<that::GameObject*> m_pPlayers{};
		that::GameObject* m_pLastPlayer{};

		std::unique_ptr<State> m_pState{};

		const float m_GridEpsilonX{ 0.5f };
		const float m_GridEpsilonY{ 1.5f };
	};
}

