#include "CameraComponent.h"

#include "Renderer.h"

void that::CameraComponent::OnDestroy()
{
	auto& renderer{ Renderer::GetInstance() };
	if(renderer.GetCamera() == this) renderer.SetCamera(nullptr);
}

void that::CameraComponent::SetAsMainCamera()
{
	Renderer::GetInstance().SetCamera(this);
}