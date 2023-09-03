#pragma once

#include "Component.h"

#include "TextureRenderer.h"

namespace that
{
	class Transform;

	class RenderComponent : public Component
	{
		SERIALIZABLE(that, RenderComponent)
		ENABLE_SERIALIZE_VAR(RenderComponent)

	public:
		RenderComponent() = default;
		virtual ~RenderComponent() = default;

		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		void SetTexture(std::shared_ptr<Texture2D> pTexture);
		void SetPivot(const glm::vec2& pivot);
		void SetPivot(float x, float y);

		glm::ivec2 GetTextureSize() const;
		glm::vec2 GetScaledTextureSize() const;
		SDL_Rect& GetSource();
		const glm::vec2& GetPivot() const;

	protected:
		const SDL_Rect& GetConstantSource() const;
		Texture2D* GetTexture() const;

	private:
		TextureRenderer m_Texture{};
	};

	SERIALIZE_VAR_START(RenderComponent)
		SERIALIZABLE_VAR(RenderComponent, m_Texture)
	SERIALIZE_VAR_END
}
