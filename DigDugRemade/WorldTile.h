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

	class WorldTile final : public that::Component, public that::Observer<glm::ivec2>
	{
	public:
		WorldTile() = default;
		virtual ~WorldTile() = default;

		WorldTile(const WorldTile& other) = delete;
		WorldTile(WorldTile&& other) = delete;
		WorldTile& operator=(const WorldTile& other) = delete;
		WorldTile& operator=(WorldTile&& other) = delete;

		virtual void Init() override;

		void BindPlayer(GridTransform* pPlayer);
		virtual void Notify(const glm::ivec2& change) override;
		virtual void OnDestroy() override;
	private:
		GridTransform* m_pGridTr{};
		GridTransform* m_pPlayer{};

		that::TextureMask* m_pLeftMask{};
		that::TextureMask* m_pRightMask{};
		that::TextureMask* m_pBottomMask{};
		that::TextureMask* m_pTopMask{};
	};
}

