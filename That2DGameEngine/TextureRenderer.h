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
		void SetScale(const glm::vec2& scale) { m_Scale = scale; }
		void SetScale(float x, float y) { m_Scale = { x,y }; }
		void SetScale(float scale) { m_Scale = { scale,scale }; }

		const glm::vec2& GetScale() const { return m_Scale; }
		glm::ivec2 GetTextureSize() const { return m_pTexture->GetSize(); }
		SDL_Rect& GetSource() { return m_SrcRect; }

		virtual void Render() const override;
	private:
		std::shared_ptr<Texture2D> m_pTexture{};
		SDL_Rect m_SrcRect{};

		glm::vec2 m_Scale{ 1.0f, 1.0f };
	};
}

