#pragma once

#include "Texture2D.h"

#include "glm/vec2.hpp"
#include "SDL_rect.h"

#include <memory>

namespace that
{
	class Transform;

	class TextureRenderer final
	{
	public:
		TextureRenderer() = default;
		~TextureRenderer() = default;

		TextureRenderer(const TextureRenderer& other) = delete;
		TextureRenderer(TextureRenderer&& other) = delete;
		TextureRenderer& operator=(const TextureRenderer& other) = delete;
		TextureRenderer& operator=(TextureRenderer&& other) = delete;

		void SetTexture(std::shared_ptr<Texture2D> pTexture);
		void SetPivot(const glm::vec2& pivot);
		void SetPivot(float x, float y);

		glm::ivec2 GetTextureSize() const { return m_pTexture->GetSize(); }
		glm::vec2 GetScaledTextureSize(Transform* pTransform) const;
		SDL_Rect& GetSource() { return m_SrcRect; }
		const SDL_Rect& GetConstantSource() const { return m_SrcRect; }
		const glm::vec2& GetPivot() const { return m_Pivot; }
		Texture2D* GetTexture() const { return m_pTexture.get(); }
	private:
		std::shared_ptr<Texture2D> m_pTexture{};
		SDL_Rect m_SrcRect{};
		glm::vec2 m_Pivot{ 0.5f, 0.5f };
	};
}

