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

void load()
{
	auto& scene = that::SceneManager::GetInstance().CreateScene("Demo");

	auto pFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	auto pFPSFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18) };

	auto pBackgroundGO = std::make_shared<that::GameObject>();
	auto pBackgroundTransform{ pBackgroundGO->AddComponent<that::Transform>() };
	auto pBackgroundTexture{ pBackgroundGO->AddComponent<that::TextureRenderer>() };
	pBackgroundTexture->SetTexture(that::ResourceManager::GetInstance().LoadTexture("background.tga"));
	scene.Add(pBackgroundGO);

	auto pLogoGO{ std::make_shared<that::GameObject>() };
	auto pLogoTransform{ pLogoGO->AddComponent<that::Transform>() };
	auto pLogoTexture{ pLogoGO->AddComponent<that::TextureRenderer>() };
	pLogoTexture->SetTexture(that::ResourceManager::GetInstance().LoadTexture("logo.tga"));
	pLogoTransform->SetPosition(216, 180);
	scene.Add(pLogoGO);

	auto pHeaderGO{ std::make_shared<that::GameObject>() };
	auto pHeaderTransform{ pHeaderGO->AddComponent<that::Transform>() };
	auto pHeaderText{ pHeaderGO->AddComponent<that::TextComponent>() };
	pHeaderGO->AddComponent<that::TextureRenderer>();
	pHeaderTransform->SetPosition(80, 20);
	pHeaderText->SetFont(pFont);
	pHeaderText->SetText("Programming 4 Assignment");
	scene.Add(pHeaderGO);

	auto go{ std::make_shared<that::GameObject>() };
	auto pTransform{ go->AddComponent<that::Transform>() };
	go->AddComponent<that::TextureRenderer>();
	auto pText{ go->AddComponent<that::TextComponent>() };
	go->AddComponent<that::FPSCounter>();
	pText->SetFont(pFPSFont);
	pText->SetColor(255, 255, 0);
	pTransform->SetPosition(0.0f, 0.0f);
	scene.Add(go);
}

int main(int, char*[]) {
	that::Engine engine("../Data/");
	engine.Run(load);
    return 0;
}