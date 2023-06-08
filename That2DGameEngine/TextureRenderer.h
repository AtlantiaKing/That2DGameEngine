#pragma once

#include "Component.h"
#include "Texture2D.h"

#include "glm/vec2.hpp"
#include "SDL_rect.h"

namespace that
{
	class Transform;

	class TextureRenderer final : public Component
	{
	public:
		TextureRenderer() = default;
		virtual ~TextureRenderer() = default;

		TextureRenderer(const TextureRenderer& other) = delete;
		TextureRenderer(TextureRenderer&& other) = delete;
		TextureRenderer& operator=(const TextureRenderer& other) = delete;
		TextureRenderer& operator=(TextureRenderer&& other) = delete;

		void SetTexture(std::shared_ptr<Texture2D> pTexture);
		void SetPivot(const glm::vec2& pivot) { m_Pivot = pivot; }

		glm::ivec2 GetTextureSize() const { return m_pTexture->GetSize(); }
		glm::vec2 GetScaledTextureSize() const;
		SDL_Rect& GetSource() { return m_SrcRect; }
		const glm::vec2& GetPivot() const { return m_Pivot; }

		virtual void Render() const override;
	private:
		std::shared_ptr<Texture2D> m_pTexture{};
		SDL_Rect m_SrcRect{};
		glm::vec2 m_Pivot{ 0.5f, 0.5f };
	};
}

