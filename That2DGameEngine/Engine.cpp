#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Engine.h"

#include "Window.h"
#include "ServiceLocator.h"
#include "SDLAudioSystem.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "EventQueue.h"
#include "Physics.h"

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

that::Engine::Engine(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	Window& window{ Window::GetInstance() };
	const bool windowCreationSucces{ window.SetWindowSize(640, 480) };

	if (!windowCreationSucces)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	ResourceManager::GetInstance().Init(dataPath);

	Timer::GetInstance().Init();

	ServiceLocator::RegisterAudioSystem<SDLAudioSystem>();
}

that::Engine::~Engine()
{
	Renderer::GetInstance().Destroy();

	SDL_Quit();
}

void that::Engine::Run(const std::function<void()>& setup)
{
	// Setup game
	setup();

	// Get singletons
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& time = Timer::GetInstance();
	auto& events = EventQueue::GetInstance();
	auto& physics = Physics::GetInstance();

	// Load the first scene
	sceneManager.LoadScene(0);

	constexpr int desiredFPS{ 60 };
	constexpr float desiredFrameTime{ 1000.0f / desiredFPS };

	// Game loop
	bool doContinue = true;
	while (doContinue)
	{
		const auto frameStart{ std::chrono::high_resolution_clock::now() };

		time.Update();
		doContinue = input.ProcessInput();

		sceneManager.OnFrameStart();
		events.NotifyListeners();

		sceneManager.Update();
		events.NotifyListeners();

		sceneManager.LateUpdate();
		events.NotifyListeners();

		physics.Update();
		events.NotifyListeners();

		sceneManager.UpdateCleanup();

		renderer.Render();

		// Sleep so that the total frame time becomes the desired frame time
		const auto frameTime{ std::chrono::high_resolution_clock::now() - frameStart };
		const auto sleepTime{ std::chrono::milliseconds(static_cast<int>(desiredFrameTime)) - frameTime };
		std::this_thread::sleep_for(sleepTime);
	}

	sceneManager.Destroy();
}
