#pragma once

#include "DataCommand.h"
#include "glm/vec2.hpp"

namespace that
{
	class Transform;
}

namespace digdug
{
	class MoveCommand final : public that::DataCommand<glm::vec2>
	{
	public:
		MoveCommand(that::Transform* pTransform, float moveSpeed);
		virtual ~MoveCommand() = default;

		virtual void Execute() override;
	private:
		that::Transform* m_pTransform{};
		const float m_MoveSpeed{};
	};
}
