#pragma once

#include "Component.h"

namespace that
{
	class CameraComponent final : public Component
	{
		SERIALIZABLE(that, CameraComponent)

	public:
		CameraComponent() = default;
		virtual ~CameraComponent() = default;

		CameraComponent(const CameraComponent& other) = delete;
		CameraComponent(CameraComponent&& other) = delete;
		CameraComponent& operator=(const CameraComponent& other) = delete;
		CameraComponent& operator=(CameraComponent&& other) = delete;

		virtual void OnDestroy() override;

		void SetAsMainCamera();
	private:
	};
}