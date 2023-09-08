#pragma once

#include <memory>
#include <string>
#include <vector>

#include "StandaloneWindow.h"

namespace that
{
	class Editor final
	{
	public:
		Editor(const std::string& dataPath);
		~Editor();

		static void UpdateVisuals();
	private:
		static Editor* m_pInstance;

		std::vector<StandaloneWindow> m_Windows{};

		void UpdateVisualsInternal();
		void QuitWindow(Uint32 windowId);
		void ClickWindow(Uint32 windowId, const glm::ivec2& point);
		void AltClickWindow(Uint32 windowId, const glm::ivec2& point);
	};
}

