#include "ScorePrefab.h"

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "FloatingScore.h"
#include "TextComponent.h"
#include "TextureRenderer.h"

#include "ResourceManager.h"

void digdug::SpawnScore(that::Scene* pScene, const glm::vec2& position, int score)
{
	that::GameObject* pScore{ pScene->CreateGameObject("Score") };
	
	pScore->AddComponent<FloatingScore>();
	pScore->AddComponent<that::TextureRenderer>();
	that::TextComponent* pText{ pScore->AddComponent<that::TextComponent>() };
	pText->SetText(std::to_string(score));
	pText->SetFont(that::ResourceManager::GetInstance().LoadFont("Fonts/Arcade.ttf", 16));

	pScore->GetTransform()->SetWorldPosition(position);
}
