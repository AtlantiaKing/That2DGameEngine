#pragma once

#include "State.h"

namespace that
{
	class GameObject;
	class SpriteRenderer;
}

namespace digdug
{
	class RockDeathState : public State
	{
	public:
		RockDeathState(that::GameObject* pRock);
		virtual ~RockDeathState() = default;

		virtual std::unique_ptr<State> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;
	private:
		that::GameObject* m_pRock{};

		that::SpriteRenderer* m_pSprite{};
	};
}


