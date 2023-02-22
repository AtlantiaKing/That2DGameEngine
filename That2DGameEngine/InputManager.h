#pragma once
#include "Singleton.h"

namespace that
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};

}
