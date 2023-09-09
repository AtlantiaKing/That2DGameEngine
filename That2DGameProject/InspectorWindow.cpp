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

#include "Editor.h"

that::InspectorWindow::InspectorWindow()
{
	m_ComponentButton.onClick = [&]() { m_IsShowingComponents = true; };
}

void that::InspectorWindow::Render(SDL_Renderer* pRenderer)
{
	GameObject* pWatchingObject{ Editor::GetInstance().GetSelectedObject() };
	if (pWatchingObject == nullptr) return;

	EditorGUI& gui{ EditorGUI::GetInstance() };

	// Get all components on the current gameobject
	const auto& goTypes{ pWatchingObject->GetComponents() };

	// Render the name of the gameobject
	int curStartY{};
	gui.RenderText(pRenderer, pWatchingObject->GetName(), curStartY);
	curStartY += 8;

	// Render each component and its variables
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

	// Render a "Add Component" button
	curStartY += 5;
	gui.RenderButton(pRenderer, "Add Component", curStartY, m_ComponentButton);
	curStartY += 5;

	// If the add component button has been pressed, render all the types known to the engine as buttons
	if (m_IsShowingComponents)
	{
		const auto& allTypes{ reflection::Reflection::GetTypes() };

		m_AddComponents.clear();
		for (const auto& type : allTypes)
		{
			Button typeButton{};
			gui.RenderButton(pRenderer, type.name, curStartY, typeButton);
			typeButton.onClick = [&, pWatchingObject]() 
			{ 
				type.Clone(pWatchingObject);
				m_IsShowingComponents = false;
			};
			m_AddComponents.emplace_back(typeButton);

			curStartY += 1;
		}
	}
}

void that::InspectorWindow::OnMouseButton(int mouseButton, bool released, const glm::ivec2& point)
{
	if (released) return;

	m_IsShowingComponents = false;

	if (mouseButton == 0)
	{
		// Delegate the click to all the buttons
		m_ComponentButton.TryClick(point);
		for (const auto& button : m_AddComponents)
		{
			button.TryClick(point);
		}
	}

	// Clear the list of buttons of AddComponent
	m_AddComponents.clear();
}