#pragma once
#include "BaseComponent.h"
#include <SDL.h>

namespace cp
{
	class Transform;
	class Texture2D final: public BaseComponent
	{
	public:
		explicit Texture2D(SDL_Texture* texture);
		virtual ~Texture2D();

		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void Draw() const override;
		virtual void DebugDraw() const override;

		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;

		// adds to the x and y of the dest rect
		void AddLocalOffset(float x, float y);
		// sets the w and h of the dest rect
		void SetLocalScale(float w, float h);
		void FlipTexture(bool flip);
		void SetDstRect(const SDL_FRect& newDstRect);
		void SetSrcRect(const SDL_Rect& newSrcRect);
		void SetPivot(const SDL_Point& newPivot);

		const SDL_FRect& GetDstRect() const;
		const SDL_Rect& GetSrcRect() const;
		const SDL_Point& GetPivot() const;
		inline SDL_Texture* GetSDLTexture() const { return m_pTexture; };

	protected:
		SDL_Texture* m_pTexture;
		SDL_Rect m_SrcRect;
		SDL_FRect m_DstRect;
		SDL_Point m_Pivot;

		bool m_FlipHorizontal;
	};
}
