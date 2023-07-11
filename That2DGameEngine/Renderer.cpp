#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Window.h"
#include "Texture2D.h"

#include "CameraComponent.h"
#include "Transform.h"

#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>
#include <iostream>
#include "glm/trigonometric.hpp"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void that::Renderer::Init(SDL_Window* window)
{
	if (!window) return;

	if (m_window)
	{
		Destroy();
	}

	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	m_RenderPosition = Window::GetInstance().GetSize() / 2;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void that::Renderer::Render()
{
	if (m_pCamera) m_RenderPosition = -m_pCamera->GetTransform()->GetWorldPosition() + static_cast<glm::vec2>(Window::GetInstance().GetSize() / 2);

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	SceneManager::GetInstance().OnGUI();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void that::Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void that::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, bool useCamera) const
{
	SDL_Rect temp{};
	constexpr glm::vec2 defaultPivot{ glm::vec2{ 0.5f, 0.5f } };
	RenderTexture(texture, temp, x, y, useCamera);
}

void that::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float scaleX, const float scaleY, float rotation, bool useCamera) const
{
	SDL_Rect temp{};
	constexpr glm::vec2 defaultPivot{ glm::vec2{ 0.5f, 0.5f } };
	RenderTexture(texture, temp, x, y, scaleX, scaleY, rotation, defaultPivot, useCamera);
}

void that::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, float x, float y, bool useCamera) const
{
	constexpr float defaultScale{ 1.0f };
	constexpr float defaultRotation{ 0.0f };
	constexpr glm::vec2 defaultPivot{ glm::vec2{ 0.5f, 0.5f } };
	RenderTexture(texture, srcRect, x, y, defaultScale, defaultScale, defaultRotation, defaultPivot, useCamera);
}

void that::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, float x, float y, float scaleX, float scaleY, float rotation, const glm::vec2& pivot, bool useCamera) const
{
	if (useCamera)
	{
		x += m_RenderPosition.x;
		y += m_RenderPosition.y;
	}

	int textureWidth{};
	int textureHeight{};
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &textureWidth, &textureHeight);

	SDL_Rect dst{};
	dst.x = static_cast<int>(x - textureWidth * abs(scaleX) * pivot.x);
	dst.y = static_cast<int>(y - textureHeight * abs(scaleY) * pivot.y);

	dst.w = static_cast<int>(textureWidth * abs(scaleX));
	dst.h = static_cast<int>(textureHeight * abs(scaleY));

	const SDL_Point rotationCenter{ static_cast<int>(dst.w * pivot.x), static_cast<int>(dst.h * pivot.y) };

	const int flipState{ (scaleX < 0.0f ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (scaleY < 0.0f ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE) };

	const bool hasSrcRect{ srcRect.w > FLT_EPSILON || srcRect.h > FLT_EPSILON };
	if (hasSrcRect)
	{
		dst.w = static_cast<int>(static_cast<float>(dst.w) * srcRect.w / textureWidth);
		dst.h = static_cast<int>(static_cast<float>(dst.h) * srcRect.h / textureHeight);

		if (static_cast<SDL_RendererFlip>(flipState) == SDL_RendererFlip::SDL_FLIP_VERTICAL)
		{
			dst.y += static_cast<int>(cosf(glm::radians(rotation)) * (textureHeight * abs(scaleY) - dst.h));
			dst.x -= static_cast<int>(sinf(glm::radians(rotation)) * (textureHeight * abs(scaleY) - dst.h));
		}
	}

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), hasSrcRect ? &srcRect : nullptr, &dst, rotation, &rotationCenter, static_cast<SDL_RendererFlip>(flipState));
}

void that::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, SDL_Rect dstRect, float rotation, bool useCamera) const
{
	if (useCamera)
	{
		dstRect.x += static_cast<int>(m_RenderPosition.x);
		dstRect.y += static_cast<int>(m_RenderPosition.y);
	}

	const SDL_Point rotationCenter{ abs(dstRect.w) / 2, abs(dstRect.h) / 2 };

	const int flipState{ (dstRect.w < 0.0f ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (dstRect.h < 0.0f ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE) };

	dstRect.w = abs(dstRect.w);
	dstRect.h = abs(dstRect.h);

	const bool hasSrcRect{ srcRect.w > FLT_EPSILON || srcRect.h > FLT_EPSILON };

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), hasSrcRect ? &srcRect : nullptr, &dstRect, rotation, &rotationCenter, static_cast<SDL_RendererFlip>(flipState));
}

void that::Renderer::DrawRect(SDL_Rect rect, const SDL_Color& color, bool useCamera)
{
	if (useCamera)
	{
		rect.x = static_cast<int>(rect.x + m_RenderPosition.x);
		rect.y = static_cast<int>(rect.y + m_RenderPosition.y);
	}

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(m_renderer, &rect);
}

inline SDL_Renderer* that::Renderer::GetSDLRenderer() const { return m_renderer; }

void that::Renderer::SetCamera(that::CameraComponent* pCamera)
{
	m_pCamera = pCamera;
}