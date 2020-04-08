#pragma once
#include "BaseComponent.h"
#include <SDL.h>

namespace cp
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Transform;
	class Texture2D final : public BaseComponent
	{
	public:
		explicit Texture2D(SDL_Texture* texture);
		virtual ~Texture2D();

		virtual void Update(float elapsedSec) override;

		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;

		void SetLocalOffset(float x, float y, float z);
		void SetDstRect(const SDL_Rect& newDstRect);
		void SetSrcRect(const SDL_Rect& newSrcRect);

		const glm::vec3& GetLocalOffset() const;
		const SDL_Rect& GetDstRect() const;
		const SDL_Rect& GetSrcRect() const;
		inline SDL_Texture* GetSDLTexture() const { return m_pTexture; };

	private:
		SDL_Texture* m_pTexture;
		Transform* m_pLocalOffset;
		SDL_Rect m_SrcRect;
		SDL_Rect m_DstRect;
	};
}
