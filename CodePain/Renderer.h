#pragma once
#include "Singleton.h"
#include <SDL.h>


namespace cp
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other) = delete;

		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dst) const;
		void RenderTexture(SDL_Texture* texture, const SDL_Rect& src, const SDL_FRect& dst) const;

		void RenderTexture(SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dst, const double angle, const SDL_Point& center, const SDL_RendererFlip flip) const;
		void RenderTexture(SDL_Texture* texture, const SDL_Rect& src, const SDL_FRect& dst, const double angle, const SDL_FPoint& center, const SDL_RendererFlip flip) const;

		void RenderRect(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const;
		void RenderCollorRect(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const;
		void RenderLine(const SDL_Point& p1, const SDL_Point& p2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

		static bool gd_RenderCollisionBoxes;
		static bool gd_RenderTextureTiles;
	private:
		SDL_Renderer* m_Renderer{};

	};
}

