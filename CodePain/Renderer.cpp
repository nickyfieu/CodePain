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
	SDL_RenderClear(m_Renderer);

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

void cp::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& src, const SDL_Rect& dst) const
{
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void cp::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& src, const SDL_FRect& dst) const
{
	SDL_RenderCopyF(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void cp::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& src, const SDL_Rect& dst, const double angle, const SDL_Point& center, const SDL_RendererFlip flip) const
{
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst,angle,&center, flip);
}

void cp::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& src, const SDL_FRect& dst, const double angle, const SDL_FPoint& center, const SDL_RendererFlip flip) const
{
	SDL_RenderCopyExF(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, angle, &center, flip);
}
