#include "CodePainPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"

#ifdef _DEBUG
	#include "Imgui\imgui.h"
	#include "Imgui_Sdl\imgui_sdl.h"
	#include "Imgui_Sdl\imgui_impl_sdl.h"
#endif

bool cp::Renderer::gd_RenderCollisionBoxes = false;
bool cp::Renderer::gd_RenderTextureTiles = false;

#define	SDL_RENDER_SCALE_QUALITY = 1;

void cp::Renderer::Init(SDL_Window * window)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC */);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

#ifdef _DEBUG
	ImGui::CreateContext();
	ImGuiSDL::Initialize(m_Renderer, 960, 500);
#endif
}

void cp::Renderer::Render() const
{
	// clear the renderer with black
	SDL_RenderClear(m_Renderer);
	SDL_RenderFillRect(m_Renderer, NULL);

	SceneManager::GetInstance().Render();

#ifdef _DEBUG
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
#endif
	
	SDL_RenderPresent(m_Renderer);
}

void cp::Renderer::Destroy()
{
#ifdef _DEBUG
	ImGuiSDL::Deinitialize();
#endif
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
#ifdef _DEBUG
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif 
}

void cp::Renderer::RenderTexture(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dst) const
{
	SDL_RenderCopy(GetSDLRenderer(), texture, &src, &dst);
}

void cp::Renderer::RenderTexture(SDL_Texture* texture, const SDL_Rect& src, const SDL_FRect& dst) const
{
	SDL_RenderCopyF(GetSDLRenderer(), texture, &src, &dst);
}

void cp::Renderer::RenderTexture(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dst, const double angle, const SDL_Point& center, const SDL_RendererFlip flip) const
{
	SDL_RenderCopyEx(GetSDLRenderer(), texture, &src, &dst,angle,&center, flip);
}

void cp::Renderer::RenderTexture(SDL_Texture* texture, const SDL_Rect& src, const SDL_FRect& dst, const double angle, const SDL_FPoint& center, const SDL_RendererFlip flip) const
{
	SDL_RenderCopyExF(GetSDLRenderer(), texture, &src, &dst, angle, &center, flip);
}

void cp::Renderer::RenderCollorRect(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
	SDL_RenderFillRect(m_Renderer, &rect);
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
}

void cp::Renderer::RenderLine(const SDL_Point& p1, const SDL_Point& p2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
	SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
	SDL_RenderDrawLine(m_Renderer, p1.x, p1.y, p2.x, p2.y);
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
}
