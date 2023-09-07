#include "InspectorWindow.h"
#include <iostream>

#include "Font.h"
#include "Texture2D.h"

#include "GameObject.h"
#include "Component.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "ResourceManager.h"

#include "Reflection.h"

that::InspectorWindow::InspectorWindow()
{
	m_ComponentButton.onClick = [&]() { m_IsShowingComponents = true; };
}

void that::InspectorWindow::Render(SDL_Renderer* pRenderer)
{
	if (m_pWatchingObject == nullptr) return;

	EditorGUI& gui{ EditorGUI::GetInstance() };

	const auto& goTypes{ m_pWatchingObject->GetComponents() };

	int curStartY{};
	gui.RenderText(pRenderer, m_pWatchingObject->GetName(), curStartY);
	curStartY += 8;

	for (const auto& type : goTypes)
	{
		const auto& typeinfo{ reflection::Reflection::GetType(type->GetHash()) };

		gui.RenderText(pRenderer, typeinfo.name, curStartY);
		curStartY += 1;

		for (const auto& member : typeinfo.variables)
		{
			gui.RenderText(pRenderer, "   " + member.name, curStartY);
			curStartY += 1;
		}
		curStartY += 3;
	}
	curStartY += 5;
	gui.RenderButton(pRenderer, "Add Component", curStartY, m_ComponentButton);
	curStartY += 5;

	m_AddComponents.clear();
	if (m_IsShowingComponents)
	{
		const auto& allTypes{ reflection::Reflection::GetTypes() };

		for (const auto& type : allTypes)
		{
			Button typeButton{};
			gui.RenderButton(pRenderer, type.name, curStartY, typeButton);
			typeButton.onClick = [&]() 
			{ 
				type.Clone(m_pWatchingObject); 
				m_IsShowingComponents = false;
			};
			m_AddComponents.emplace_back(typeButton);

			curStartY += 1;
		}
	}
}

void that::InspectorWindow::OnClick(const glm::ivec2& point)
{
	m_IsShowingComponents = false;

	m_ComponentButton.TryClick(point);
	for (const auto& button : m_AddComponents)
	{
		button.TryClick(point);
	}
}

void that::InspectorWindow::SetGameObject(GameObject* pGameObject)
{
	m_pWatchingObject = pGameObject;
}