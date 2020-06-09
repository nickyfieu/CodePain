#pragma once
#include "BaseComponent.h"
#include <SDL.h>

namespace cp
{
	// COLOR RECT 2D
	class ColorRect2D final : public BaseComponent
	{
	public:
		// to initialize a Uint32 to be rgba correct
		// 0b 0000 0000 0000 0000 0000 0000 0000 0000
		// rgba = (rgba | a) << 8
		// 0b 0000 0000 0000 0000 AAAA AAAA 0000 0000
		// rgba = (rgba | b) << 8
		// 0b 0000 0000 AAAA AAAA BBBB BBBB 0000 0000
		// rgba = (rgba | g) << 8
		// 0b AAAA AAAA BBBB BBBB GGGG GGGG 0000 0000
		// rgba = (rgba | r) << 0
		// 0b AAAA AAAA BBBB BBBB GGGG GGGG RRRR RRRR
		ColorRect2D(int x, int y, int width, int height, Uint32 rgba);
		virtual ~ColorRect2D() = default;

		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void DebugDraw() const override;
		virtual void Draw() const override;

		ColorRect2D(const ColorRect2D& other) = delete;
		ColorRect2D(ColorRect2D&& other) = delete;
		ColorRect2D& operator=(const ColorRect2D& other) = delete;
		ColorRect2D& operator=(ColorRect2D&& other) = delete;

		inline const SDL_Rect& GetColorBox() const { return m_ColorBox; }

		// rgba = 0b AAAA AAAA BBBB BBBB GGGG GGGG RRRR RRRR
		// to read  r / g / b / a
		// r = Uint8(rgba >> 0 )
		// g = Uint8(rgba >> 8 )
		// b = Uint8(rgba >> 16 )
		// a = Uint8(rgba >> 24 )
		inline Uint32 GetRGBA() const { return m_RGBA; }
	private:
		const SDL_Rect m_ColorBox;
		const Uint32  m_RGBA;
	};
}