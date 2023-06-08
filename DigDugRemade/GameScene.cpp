#include "GameScene.h"

// Objects
#include "Scene.h"
#include "GameObject.h"

// Components
#include "Transform.h"
#include "LevelLoader.h"
#include "GridComponent.h"

// Engine includes
#include "Window.h"

// Library includes
#include <iostream>
#include "glm/vec2.hpp"

void digdug::GameScene::Load(that::Scene& scene)
{
	// Grid
	that::GameObject* pGrid{ scene.CreateGameObject("Grid") };
	pGrid->AddComponent<LevelLoader>()->SetLevel("Levels/DemoLevel.png");

	PrintControls();
}

void digdug::GameScene::PrintControls()
{
	std::cout << "\n";
	std::cout << "Controls for DigDug Remade\n";
	std::cout << "\n";
	std::cout << "Move: WASD\n";
	std::cout << "Shoot pump: Space bar\n";
	std::cout << "\n";
	std::cout << "Sounds in the current game:\n";
	std::cout << "Music while walking\n";
	std::cout << "Sounds when you hit an enemy and a different sound when you kill the enemy\n";
	std::cout << "\n";
}
