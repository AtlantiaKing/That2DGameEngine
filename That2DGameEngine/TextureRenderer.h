#pragma once

#include "Component.h"
#include "Texture2D.h"

#include "glm/vec2.hpp"

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

		virtual void Render() const override;
	private:
		std::shared_ptr<Texture2D> m_pTexture{};

		glm::vec2 m_Scale{};
	};
}

