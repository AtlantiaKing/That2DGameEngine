#pragma once

#include "State.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class GridComponent;

	class RockStaticState : public State
	{
	public:
		RockStaticState(that::GameObject* pRock);
		virtual ~RockStaticState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;
	private:
		that::GameObject* m_pRock{};

		GridComponent* m_pGrid{};
	};
}



