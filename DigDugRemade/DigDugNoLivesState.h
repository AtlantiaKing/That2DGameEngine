#pragma once

#include "DigDugState.h"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class DigDugNoLivesState final : public DigDugState
	{
	public:
		DigDugNoLivesState(that::GameObject* pPlayer);
		virtual ~DigDugNoLivesState() = default;

		virtual std::unique_ptr<DigDugState> HandleInput(const glm::ivec2&, bool, bool);
		virtual std::unique_ptr<DigDugState> Update();
		virtual void StateEnter();
		virtual void StateEnd();
	private:
		that::GameObject* m_pPlayer{};
	};
}
