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

void that::InspectorWindow::Render(SDL_Renderer* pRenderer)
{
	GameObject* pWatchingObject{ Editor::GetInstance().GetSelectedObject() };
	if (pWatchingObject == nullptr) return;

	// Get all components on the current gameobject
	const auto& goTypes{ pWatchingObject->GetComponents() };
	
	EditorGUI& gui{ EditorGUI::GetInstance() };
	gui.Begin(pRenderer);

	// Render the name of the gameobject
	gui.RenderInputField(pWatchingObject->GetName());

	gui.Space(8);

	const auto& types{ reflection::Reflection::GetBasicTypes() };

	// Render each component and its variables
	for (const auto& type : goTypes)
	{
		const auto& typeinfo{ reflection::Reflection::GetType(type->GetHash()) };

		gui.RenderMultiButtonFullWidth(typeinfo.name);

		for (const auto& member : typeinfo.variables)
		{
			gui.RenderText("   " + member.name);

			const auto& typeIt{ std::find_if(begin(types), end(types), [&](const auto& type) { return type.hash == member.hash; }) };
			if (typeIt != end(types))
			{
				if (typeIt->underlyingTypes.empty())
				{
					gui.RenderText("   " + typeIt->dataToString(type + member.offset));
				}
				else
				{
					size_t underlyingOffset{};
					for (const auto& underlying : typeIt->underlyingTypes)
					{
						gui.RenderText("   " + underlying.first + ": " + underlying.second.dataToString(reinterpret_cast<char*>(type) + member.offset + underlyingOffset));
						underlyingOffset += underlying.second.size;
					}
				}
			}

			gui.Space(3);
		}

		gui.Space(5);
	}

	// Render a "Add Component" button
	gui.Space(5);

	if (gui.RenderButton("Add Component")) m_IsShowingComponents = true;

	if (m_IsShowingComponents)
	{
		gui.Space(5);

		// If the add component button has been pressed, render all the types known to the engine as buttons
		const auto& allTypes{ reflection::Reflection::GetTypes() };

		for (const auto& type : allTypes)
		{
			if(gui.RenderMultiButtonFullWidth(type.name) == 0) 
			{
				type.Clone(pWatchingObject);
				m_IsShowingComponents = false;
			}
		}
	}
}