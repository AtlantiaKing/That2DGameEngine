#pragma once

#include "Component.h"

namespace digdug
{
	class BackToMainMenu final : public that::Component
	{
	public:

		virtual void Init() override;
		virtual void OnDestroy() override;
	};
}
