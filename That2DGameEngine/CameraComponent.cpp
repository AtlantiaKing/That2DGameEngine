#include "CameraComponent.h"

#include "Renderer.h"

void that::CameraComponent::SetAsMainCamera()
{
	Renderer::GetInstance().SetCamera(this);
}