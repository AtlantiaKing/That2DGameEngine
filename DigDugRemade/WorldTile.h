#pragma once

#include "Component.h"
#include "Observer.h"

#include "glm/vec2.hpp"

namespace that
{
	class TextureMask;
}

namespace digdug
{
	class GridTransform;
	class GridComponent;

	class WorldTile final : public that::Component
	{
	public:
		WorldTile() = default;
		virtual ~WorldTile() = default;

		WorldTile(const WorldTile& other) = delete;
		WorldTile(WorldTile&& other) = delete;
		WorldTile& operator=(const WorldTile& other) = delete;
		WorldTile& operator=(WorldTile&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;

		void UpdatePlayer(const glm::ivec2& playerCell, const glm::ivec2& playerPosition, const glm::ivec2& direction, float playerSize);
		void SetMasks(float left, float right, float bottom, float top);

		bool IsValidPosition(const glm::vec2& position, const glm::ivec2& direction, float size) const;
		bool IsOpen() const;
		glm::ivec2 GetGridPosition() const;

	private:
		GridComponent* m_pGrid{};
		GridTransform* m_pGridTr{};

		that::TextureMask* m_pLeftMask{};
		that::TextureMask* m_pRightMask{};
		that::TextureMask* m_pBottomMask{};
		that::TextureMask* m_pTopMask{};

		const float m_BorderSize{ 2.0f };
		const float m_UnusedMaskEpsilon{ 0.1f };
	};
}

