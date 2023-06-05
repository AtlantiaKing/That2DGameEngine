#include "MainMenuFadeIn.h"

#include "Transform.h"

#include "LambdaCommand.h"

#include "Window.h"
#include "Timer.h"
#include "InputManager.h"

#include "SDL_keyboard.h"

void digdug::MainMenuFadeIn::Init()
{
	// Bind the end face method to the enter button
	that::InputManager::GetInstance().BindDigitalCommand
	(
		SDLK_RETURN,
		that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>(
		[this]()
		{
			EndFade();
		})
	);
}

void digdug::MainMenuFadeIn::Update()
{
	// Calculate the go to y position
	const float goToY{ that::Window::GetInstance().GetSize().y / 2.0f };

	// Get the current transform position
	that::Transform* pTransform{ GetTransform() };
	const auto& curPosition{ pTransform->GetWorldPosition() };

	// Translate the main menu upwards
	const float newY{ curPosition.y - m_FadeSpeed * that::Timer::GetInstance().GetElapsed() };
	pTransform->SetWorldPosition(curPosition.x, newY);

	// End the fade in when the main menu surpasses the go to position
	if(newY < goToY)
	{
		EndFade();
	}
}

void digdug::MainMenuFadeIn::EndFade()
{
	// Reset the main menu to the middle of the screen
	const auto& windowSize{ that::Window::GetInstance().GetSize() };
	GetTransform()->SetWorldPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	// Clear all bound input
	that::InputManager::GetInstance().Clear();

	// Notify any listeners
	OnFadeComplete.Notify(*this);

	// Delete this component
	Destroy();
}
