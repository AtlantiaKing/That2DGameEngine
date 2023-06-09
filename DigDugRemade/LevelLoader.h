#pragma once

#include "Scene.h"
#include "Component.h"

#include <string>

namespace that
{
	class GameObject;
}

namespace digdug
{
	class LevelLoader final : public that::Component
	{
	public:
		LevelLoader() = default;
		virtual ~LevelLoader() = default;

		LevelLoader(const LevelLoader& other) = delete;
		LevelLoader(LevelLoader&& other) = delete;
		LevelLoader& operator=(const LevelLoader& other) = delete;
		LevelLoader& operator=(LevelLoader&& other) = delete;

		void SetLevel(const std::string& filePath, int nrPlayers);

		virtual void Init() override;
		virtual void OnFrameStart() override;

	private:
		that::GameObject* CreatePlayer(int index);

		std::string m_Level{};
		int m_NrPlayers{};

		const int m_NoMapValue{ 1 };
		const int m_EnemyHealth{ 4 };
	};
}