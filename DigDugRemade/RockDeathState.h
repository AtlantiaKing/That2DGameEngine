#pragma once

#include "EnemyState.h"

namespace that
{
	class GameObject;
	class SpriteRenderer;
}

namespace digdug
{
	class RockDeathState : public EnemyState
	{
	public:
		RockDeathState(that::GameObject* pRock);
		virtual ~RockDeathState() = default;

		virtual std::unique_ptr<EnemyState> Update() override;
		virtual void StateEnter() override;
		virtual void StateEnd() override;
	private:
		that::GameObject* m_pRock{};

		that::SpriteRenderer* m_pSprite{};
	};
}


