#pragma once

#include "DataCommand.h"
#include "glm/vec2.hpp"

namespace that
{
	class GameObject;

	class MoveCommand final : public DataCommand<glm::vec2>
	{
	public:
		MoveCommand(GameObject* pPlayer, float moveSpeed) : m_pPlayer{ pPlayer }, m_MoveSpeed{ moveSpeed } {}
		virtual ~MoveCommand() = default;

		virtual void Execute() override;
	private:
		GameObject* m_pPlayer{};
		const float m_MoveSpeed{ 10.0f };
	};
}

