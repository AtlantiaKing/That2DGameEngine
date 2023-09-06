#pragma once

#include "Component.h"
#include "Texture2D.h"

#include "glm/vec2.hpp"
#include "SDL_rect.h"

namespace that
{
	class Transform;

	class SpriteRenderer final : public Component
	{
		COMPONENT(SpriteRenderer)
		ENABLE_SERIALIZE_VAR(SpriteRenderer)

	public:
		SpriteRenderer() = default;
		virtual ~SpriteRenderer() = default;

		SpriteRenderer(const SpriteRenderer& other) = delete;
		SpriteRenderer(SpriteRenderer&& other) = delete;
		SpriteRenderer& operator=(const SpriteRenderer& other) = delete;
		SpriteRenderer& operator=(SpriteRenderer&& other) = delete;

		void SetSprite(std::shared_ptr<Texture2D> pTexture, int tilesInX, int tilesInY, float timePerTile = FLT_MAX);
		void SetTexture(std::shared_ptr<Texture2D> pTexture, bool reset = true);
		void SetTimePerTile(float timePerTile, bool reset = true);
		void SetTile(int tile);

		glm::ivec2 GetTextureSize() const;
		glm::vec2 GetScaledTextureSize() const;
		int GetTile() const;

		virtual void Update() override;
		virtual void Render() const override;
	private:
		void Reset();
		void RecalculateSrcRect();

		std::shared_ptr<Texture2D> m_pTexture{};
		SDL_Rect m_SrcRect{};

		float m_CurTime{};
		float m_TimeBetweenTiles{};

		int m_CurTile{};
		int m_NrTiles{};
		int m_NrTilesX{};
	};

	SERIALIZE_VAR_START(SpriteRenderer)
		SERIALIZABLE_VAR(SpriteRenderer, m_pTexture)
		SERIALIZABLE_VAR(SpriteRenderer, m_CurTime)
		SERIALIZABLE_VAR(SpriteRenderer, m_TimeBetweenTiles)
	SERIALIZE_VAR_END
}

