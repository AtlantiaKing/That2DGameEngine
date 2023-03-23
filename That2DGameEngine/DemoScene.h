#pragma once

#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextureRenderer.h"
#include "Transform.h"
#include "TextComponent.h"
#include "FPSCounter.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "GameObject.h"
#include "Scene.h"

namespace that
{
	void LoadDemoScene(Scene& scene)
	{
		// Load fonts and textures used in the scene
		const auto pFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
		const auto pFPSFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18) };
		const auto pBackgroundTexture{ that::ResourceManager::GetInstance().LoadTexture("background.tga") };
		const auto pLogoTexture{ that::ResourceManager::GetInstance().LoadTexture("logo.tga") };

		// Create a GO that renders the background texture
		const auto pBackgroundGO{ scene.CreateGameObject("Background") };
		pBackgroundGO->AddComponent<that::TextureRenderer>()->SetTexture(pBackgroundTexture);

		// Create a GO that renders the logo texture
		const auto pLogoGO{ scene.CreateGameObject("DAELogo") };
		pLogoGO->GetComponent<that::Transform>()->SetWorldPosition(216, 180);
		pLogoGO->AddComponent<that::TextureRenderer>()->SetTexture(pLogoTexture);

		// Create a GO that renders a text on the screen
		const auto pHeaderGO{ scene.CreateGameObject("Header") };
		pHeaderGO->GetComponent<that::Transform>()->SetWorldPosition(80, 20);
		pHeaderGO->AddComponent<that::TextureRenderer>();
		const auto pHeaderText{ pHeaderGO->AddComponent<that::TextComponent>() };
		pHeaderText->SetFont(pFont);
		pHeaderText->SetText("Programming 4 Assignment");

		// Create a GO that renders the FPS on the screen
		const auto pFpsText{ scene.CreateGameObject("FPS counter") };
		pFpsText->GetComponent<that::Transform>()->SetWorldPosition(0.0f, 0.0f);
		pFpsText->AddComponent<that::TextureRenderer>();
		pFpsText->AddComponent<that::FPSCounter>();
		const auto pText{ pFpsText->AddComponent<that::TextComponent>() };
		pText->SetFont(pFPSFont);
		pText->SetColor(255, 255, 0);

		// Create a GO that renders the main character and rotates around the empty pivot object
		const auto pCharacter{ scene.CreateGameObject("Character") };
		pCharacter->AddComponent<that::TextureRenderer>()->SetTexture(that::ResourceManager::GetInstance().LoadTexture("MainCharacter.png"));
		pCharacter->GetTransform()->SetWorldPosition(200, 200);

		// Create a GO that renders the enemy and rotates around the character at double speed
		const auto pEnemy{ scene.CreateGameObject("Enemy") };
		pEnemy->AddComponent<that::TextureRenderer>()->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Enemy.png"));
		pEnemy->GetTransform()->SetWorldPosition(300, 300);

		// Bind the move command to WASD
		std::vector<unsigned int> character1Input{ SDLK_d,	SDLK_a,	SDLK_w,	SDLK_s };
		that::InputManager::GetInstance().BindDigital2DAxisCommand(character1Input, std::make_unique<that::MoveCommand>(pCharacter, 100.0f));

		// Bind the move command to the DPAD of the gamepad
		std::vector<that::InputManager::GamepadInput> character2Input
		{
			that::InputManager::GamepadInput::DPAD_RIGHT,
			that::InputManager::GamepadInput::DPAD_LEFT,
			that::InputManager::GamepadInput::DPAD_UP,
			that::InputManager::GamepadInput::DPAD_DOWN
		};
		that::InputManager::GetInstance().BindDigital2DAxisCommand(0, character2Input, std::make_unique<that::MoveCommand>(pEnemy, 200.0f));
	}
}