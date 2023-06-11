#pragma once

#include "Component.h"

namespace that
{
	class TextureMask;
	class TextureRenderer;
}

namespace digdug
{
	class GridComponent;

	class DisableOnGroundHit final : public that::Component
	{
	public:
		DisableOnGroundHit() = default;
		virtual ~DisableOnGroundHit() = default;

		DisableOnGroundHit(const DisableOnGroundHit& other) = delete;
		DisableOnGroundHit(DisableOnGroundHit&& other) = delete;
		DisableOnGroundHit& operator=(const DisableOnGroundHit& other) = delete;
		DisableOnGroundHit& operator=(DisableOnGroundHit&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;

	private:
		that::TextureMask* m_pMask{};
		that::TextureRenderer* m_pTexture{};
		GridComponent* m_pGrid{};
	};
}

