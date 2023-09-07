#pragma once

#include "WindowComponent.h"

namespace that
{
	class SceneWindow final : public WindowComponent
	{
	public:
		SceneWindow() = default;
		virtual ~SceneWindow() = default;

		virtual void Render(SDL_Renderer* pWindow) override;

	private:
		bool m_Initialised{};
	};
}
