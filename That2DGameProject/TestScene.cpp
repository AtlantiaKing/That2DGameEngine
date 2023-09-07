#include "TestScene.h"

#include "Scene.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "Transform.h"

#include "InputManager.h"

#include "LambdaCommand.h"

#include <memory>

#include "Physics.h"

#include "SceneSerialization.h"
#include <SDL_keycode.h>
#include "SceneManager.h"

void digdug::TestScene::Load(that::Scene& scene)
{
	that::Physics::GetInstance().ActivateDebugRendering(true);

	auto ground = scene.CreateGameObject("ground");
	auto groundBox = ground->AddComponent<that::BoxCollider>();
	groundBox->SetSize(300, 300);
	//groundBox->SetMaterial(that::PhysicsMaterial{ 0.4f, 1.0f });
	ground->GetTransform()->SetLocalPosition(150, 350);

	that::InputManager::GetInstance().BindDigitalCommand(SDLK_PAGEUP, that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([]()
			{
				that::SceneManager::GetInstance().LoadScene(1);
			}));

	auto ground2 = scene.CreateGameObject("ground2");
	auto groundBox2 = ground2->AddComponent<that::BoxCollider>();
	groundBox2->SetSize(300, 300);
	//groundBox2->SetMaterial(that::PhysicsMaterial{ 0.4f, 1.0f });
	ground2->GetTransform()->SetLocalPosition(500, 350);


	auto ground3 = ground2->CreateGameObject("ground3");
	auto groundBox3 = ground3->AddComponent<that::BoxCollider>();
	groundBox3->SetSize(50, 50);
	//groundBox2->SetMaterial(that::PhysicsMaterial{ 0.4f, 1.0f });
	ground3->GetTransform()->SetLocalPosition(100, 100);

	auto player = scene.CreateGameObject("player");
	auto playerBox = player->AddComponent<that::BoxCollider>();
	playerBox->SetSize(10, 20);
	playerBox->SetMaterial(that::PhysicsMaterial{ 0.0f, 0.4f, that::PhysicsMaterial::PropertyMixer::Maximum, that::PhysicsMaterial::PropertyMixer::Maximum });
	auto rb = player->AddComponent<that::Rigidbody>();
	rb->SetGravity(100);
	player->GetTransform()->SetLocalPosition(25, 0);

	that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>([rb]() { rb->AddForce({ 0,-60 }, that::Rigidbody::ForceMode::Impulse); }));
	that::InputManager::GetInstance().BindDigitalCommand(' ', that::InputManager::InputType::ONBUTTON,
		std::make_unique<that::LambdaCommand>([rb]() { rb->AddForce({ 0,-200 }, that::Rigidbody::ForceMode::Force); }));
	that::InputManager::GetInstance().BindDigitalCommand('d', that::InputManager::InputType::ONBUTTON,
		std::make_unique<that::LambdaCommand>([rb]() { rb->AddForce({ 60,0 }, that::Rigidbody::ForceMode::Force); }));
	that::InputManager::GetInstance().BindDigitalCommand('q', that::InputManager::InputType::ONBUTTON,
		std::make_unique<that::LambdaCommand>([rb]() { rb->AddForce({ -60,0 }, that::Rigidbody::ForceMode::Force); }));

	that::reflection::SceneSerialization::SerializeScene(&scene);
	std::cout << "Writing to file\n";
}
