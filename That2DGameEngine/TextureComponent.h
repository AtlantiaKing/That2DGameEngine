#pragma once

#include "RenderComponent.h"


namespace that
{
	class Transform;

	class TextureComponent final : public RenderComponent
	{
		COMPONENT(TextureComponent)

	public:
		TextureComponent() = default;
		virtual ~TextureComponent() = default;

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		virtual void Render() const override;
	private:
	};
}

