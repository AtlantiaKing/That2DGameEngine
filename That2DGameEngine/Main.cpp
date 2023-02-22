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

	constexpr int fontSize{ 30 };
	auto pFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize) };

	auto go{ std::make_shared<that::GameObject>() };
	auto pTransform{ go->AddComponent<that::Transform>() };
	go->AddComponent<that::TextureRenderer>();
	auto pText{ go->AddComponent<that::TextComponent>() };
	go->AddComponent<that::FPSCounter>();
	pText->SetText("This is a test message");
	pText->SetFont(pFont);
	pText->SetColor(255, 0, 0);
	pTransform->SetPosition(200.0f, 200.0f);
	scene.Add(go);

	/*auto go = std::make_shared<that::GameObject>();
	go->SetTexture("background.tga");
	scene.Add(go);

	go = std::make_shared<that::GameObject>();
	go->SetTexture("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<that::TextObject>("Programming 4 Assignment", font);
	to->SetPosition(80, 20);
	scene.Add(to);*/
}

int main(int, char*[]) {
	that::Engine engine("../Data/");
	engine.Run(load);
    return 0;
}