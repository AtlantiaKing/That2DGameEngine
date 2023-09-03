#pragma once

#include "RenderComponent.h"

namespace that
{
	class UIComponent final : public RenderComponent
	{
		SERIALIZABLE(that, UIComponent)

	public:
		UIComponent() = default;
		virtual ~UIComponent() = default;

		UIComponent(const UIComponent& other) = delete;
		UIComponent(UIComponent&& other) = delete;
		UIComponent& operator=(const UIComponent& other) = delete;
		UIComponent& operator=(UIComponent&& other) = delete;

		virtual void Render() const override;
	};
}
