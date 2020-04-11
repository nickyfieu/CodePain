#pragma once
#include "BaseComponent.h"
#include <SDL.h>

namespace cp
{
	class ColRect2D final : public BaseComponent
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
		ColRect2D(int x, int y, int width, int height, Uint32 rgba);
		virtual ~ColRect2D() = default;

		virtual void Update(float elapsedSec) override;

		ColRect2D(const ColRect2D& other) = delete;
		ColRect2D(ColRect2D&& other) = delete;
		ColRect2D& operator=(const ColRect2D& other) = delete;
		ColRect2D& operator=(ColRect2D&& other) = delete;

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