#pragma once

#include "WindowComponent.h"

#include <string>
#include <memory>
#include <functional>

#include "EditorGUI.h"

namespace that
{
	class GameObject;

	class InspectorWindow final : public WindowComponent
	{
	public:
		InspectorWindow() = default;
		virtual ~InspectorWindow() = default;

		virtual void Render(SDL_Renderer* pWindow) override;

	private:
		bool m_IsShowingComponents{ false };
	};
}
