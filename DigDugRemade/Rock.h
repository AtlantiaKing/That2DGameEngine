#pragma once

#include "Component.h"

#include "EnemyState.h"

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

		void Start(that::GameObject* pPlayer);
	private:
		void ChangeState(std::unique_ptr<digdug::EnemyState> pState);

		std::unique_ptr<EnemyState> m_pState{};
	};
}

