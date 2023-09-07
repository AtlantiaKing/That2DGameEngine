#include "FileScene.h"

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

void digdug::FileScene::Load(that::Scene& scene)
{
	that::Physics::GetInstance().ActivateDebugRendering(true);

	that::reflection::SceneSerialization::DeserializeScene(&scene);
	std::cout << "Reading from file\n";
}
