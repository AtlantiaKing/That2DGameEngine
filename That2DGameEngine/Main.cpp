#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "FPSCounter.h"
#include "RotatorComponent.h"
#include "TrashTheCacheComponent.h"
#include "InputManager.h"
#include "MoveCommand.h"

void load()
{
	// TODO: Move scene loading to a class

	//// Create a new scene
	//auto& scene = that::SceneManager::GetInstance().CreateScene("TrashTheCache");

	//that::Renderer::GetInstance().SetBackgroundColor(SDL_Color{ 62, 90, 111 });

	//// Load fonts and textures used in the scene
	//const auto pFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };

	//// Create a GO that renders a text on the screen
	//const auto pHeaderGO{ scene.CreateGameObject("Header") };
	//pHeaderGO->GetComponent<that::Transform>()->SetWorldPosition(80, 20);
	//pHeaderGO->AddComponent<that::TextureRenderer>();
	//const auto pHeaderText{ pHeaderGO->AddComponent<that::TextComponent>() };
	//pHeaderText->SetFont(pFont);
	//pHeaderText->SetText("Programming 4 Assignment");

	//// Create a GO that renders the Trash The Cache assignment
	//const auto pTrashTheCache{ scene.CreateGameObject("AssignmentObject")};
	//pTrashTheCache->AddComponent<that::TrashTheCacheComponent>();

	// Create a new scene
	auto& scene = that::SceneManager::GetInstance().CreateScene("Demo");

	// Load fonts and textures used in the scene
	const auto pFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	const auto pFPSFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18) };
	const auto pBackgroundTexture{ that::ResourceManager::GetInstance().LoadTexture("background.tga") };
	const auto pLogoTexture{ that::ResourceManager::GetInstance().LoadTexture("logo.tga") };

	// Create a GO that renders the background texture
	const auto pBackgroundGO{ scene.CreateGameObject("Background")};
	pBackgroundGO->AddComponent<that::TextureRenderer>()->SetTexture(pBackgroundTexture);

	// Create a GO that renders the logo texture
	const auto pLogoGO{ scene.CreateGameObject("DAELogo") };
	pLogoGO->GetComponent<that::Transform>()->SetWorldPosition(216, 180);
	pLogoGO->AddComponent<that::TextureRenderer>()->SetTexture(pLogoTexture);

	// Create a GO that renders a text on the screen
	const auto pHeaderGO{ scene.CreateGameObject("Header")};
	pHeaderGO->GetComponent<that::Transform>()->SetWorldPosition(80, 20);
	pHeaderGO->AddComponent<that::TextureRenderer>();
	const auto pHeaderText{ pHeaderGO->AddComponent<that::TextComponent>() };
	pHeaderText->SetFont(pFont);
	pHeaderText->SetText("Programming 4 Assignment");

	// Create a GO that renders the FPS on the screen
	const auto pFpsText{ scene.CreateGameObject("FPS counter")};
	pFpsText->GetComponent<that::Transform>()->SetWorldPosition(0.0f, 0.0f);
	pFpsText->AddComponent<that::TextureRenderer>();
	pFpsText->AddComponent<that::FPSCounter>();
	const auto pText{ pFpsText->AddComponent<that::TextComponent>() };
	pText->SetFont(pFPSFont);
	pText->SetColor(255, 255, 0);

	// Create a GO that renders the main character and rotates around the empty pivot object
	const auto pCharacter{ scene.CreateGameObject("Character")};
	pCharacter->AddComponent<that::TextureRenderer>()->SetTexture(that::ResourceManager::GetInstance().LoadTexture("MainCharacter.png"));
	pCharacter->GetTransform()->SetWorldPosition(200, 200);

	// Create a GO that renders the enemy and rotates around the character at double speed
	const auto pEnemy{ scene.CreateGameObject("Enemy")};
	pEnemy->AddComponent<that::TextureRenderer>()->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Enemy.png"));
	pEnemy->GetTransform()->SetWorldPosition(300, 300);

	// Bind the move command to the DPAD of the gamepad
	std::vector<unsigned int> character1Input{ XINPUT_GAMEPAD_DPAD_RIGHT, XINPUT_GAMEPAD_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_UP, XINPUT_GAMEPAD_DPAD_DOWN };
;	that::InputManager::GetInstance().BindDigital2DAxisCommand(0, character1Input, std::make_unique<that::MoveCommand>(pCharacter, 100.0f));

	// Bind the move command of the right joystick of the gamepad
	that::InputManager::GetInstance().BindAnalog2DAxisCommand(0, true, std::make_unique<that::MoveCommand>(pEnemy, 100.0f));
}

int main(int, char*[]) {
	that::Engine engine("../Data/");
	engine.Run(load);
    return 0;
}