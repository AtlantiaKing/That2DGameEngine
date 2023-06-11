#pragma once

#include "FygarLogic.h"

#include "glm/vec2.hpp"

namespace that
{
	class GameObject;
}

namespace digdug
{
	class Fygar;

	class FygarGhostAI final : public FygarLogic
	{
	public:
		FygarGhostAI(that::GameObject* pFygar);
		virtual ~FygarGhostAI() = default;

		virtual std::unique_ptr<State> Update() override;

	private:
		that::GameObject* m_pFygarObj{};
		Fygar* m_pFygar{};

		const float m_MoveSpeed{ 25.0f };
	};
}


