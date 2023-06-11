#include "GameModeChooser.h"

#include "Transform.h"

#include "LambdaCommand.h"

#include "InputManager.h"
#include "SceneManager.h"

#include "Logger.h"
#include "GameData.h"

void digdug::GameModeChooser::SetMarkerPositions(const glm::vec2& marker1, const glm::vec2& marker2, const glm::vec2& marker3)
{
	// If the markers were already set, clear the list
	if (!m_Markers.empty()) m_Markers.clear();

	// Add the markers to the list
	m_Markers.emplace_back(marker1);
	m_Markers.emplace_back(marker2);
	m_Markers.emplace_back(marker3);

	// Update the current position of the marker
	UpdatePosition();
}

void digdug::GameModeChooser::MarkerUp()
{
	// Log a warning of no markers have been set yet
	if (m_Markers.empty())
	{
		that::Logger::LogWarning("No markers set", GetOwner());
		return;
	}

	// Decrement the current marker
	--m_CurMarker;
	// Clamp between min and max value
	if (m_CurMarker < 0) m_CurMarker = static_cast<int>(m_Markers.size()) - 1;

	// Update the current position of the marker
	UpdatePosition();
}

void digdug::GameModeChooser::MarkerDown()
{
	// Log a warning of no markers have been set yet
	if (m_Markers.empty())
	{
		that::Logger::LogWarning("No markers set", GetOwner());
		return;
	}

	// Increment and clamp the current marker
	++m_CurMarker %= static_cast<int>(m_Markers.size());

	// Update the current position of the marker
	UpdatePosition();
}

void digdug::GameModeChooser::BindInput()
{
	// Bind the enter button to a scene load
	that::InputManager::GetInstance().BindDigitalCommand
	(
		' ',
		that::InputManager::InputType::ONBUTTONDOWN, 
		std::make_unique<that::LambdaCommand>(
		[this]() 
		{
			LoadScene();

			GameData::GetInstance().ResetGame();
		}
	));
	that::InputManager::GetInstance().BindDigitalCommand
	(
		0, that::InputManager::GamepadButton::A,
		that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>(
			[this]()
			{
				LoadScene();

				GameData::GetInstance().ResetGame();
			}
	));
	that::InputManager::GetInstance().BindDigitalCommand
	(
		1, that::InputManager::GamepadButton::A,
		that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>(
			[this]()
			{
				LoadScene();

				GameData::GetInstance().ResetGame();
			}
	));

	// Bind the arrow keys to marker changes
	that::InputManager::GetInstance().BindDigitalCommand
	(
		'w',
		that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>(
			[this]()
			{
				MarkerUp();
			}
	));
	that::InputManager::GetInstance().BindDigitalCommand
	(
		0, that::InputManager::GamepadButton::DPAD_UP,
		that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>(
			[this]()
			{
				MarkerUp();
			}
	));
	that::InputManager::GetInstance().BindDigitalCommand
	(
		1, that::InputManager::GamepadButton::DPAD_UP,
		that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>(
			[this]()
			{
				MarkerUp();
			}
	));

	that::InputManager::GetInstance().BindDigitalCommand
	(
		's',
		that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>(
			[this]()
			{
				MarkerDown();
			}
	));
	that::InputManager::GetInstance().BindDigitalCommand
	(
		0, that::InputManager::GamepadButton::DPAD_DOWN,
		that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>(
			[this]()
			{
				MarkerDown();
			}
	));
	that::InputManager::GetInstance().BindDigitalCommand
	(
		1, that::InputManager::GamepadButton::DPAD_DOWN,
		that::InputManager::InputType::ONBUTTONDOWN,
		std::make_unique<that::LambdaCommand>(
			[this]()
			{
				MarkerDown();
			}
	));
}

void digdug::GameModeChooser::UpdatePosition()
{
	// Move the marker to the current marker position
	GetTransform()->SetLocalPosition(m_Markers[m_CurMarker]);
}

void digdug::GameModeChooser::LoadScene()
{
	// Log a warning if no markers have been set
	if (m_Markers.empty())
	{
		that::Logger::LogWarning("No markers set", GetOwner());
		return;
	}

	// Clear the input of this scene
	that::InputManager::GetInstance().Clear();
	// Load the correct scene
	that::SceneManager::GetInstance().LoadScene(m_CurMarker + 1);
}
