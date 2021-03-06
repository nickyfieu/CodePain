#pragma once
#include "Transform.h"
#include <SDL.h>

namespace cp
{
	class Font;
	class Texture2D;
	class Text final : public BaseComponent
	{
	public:
		Text(const std::string& text, Font* font, SDL_Color color = {255,255,255});
		virtual ~Text();
	
		virtual void Update(float elapsedSec) override;
		virtual void FixedUpdate(float elapsedSec) override;
		virtual void Draw() const override;
		virtual void DebugDraw() const override;
	
		Text(const Text& other) = delete;
		Text(Text&& other) = delete;
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) = delete;

		void SetText(const std::string& text);
		void SetTextColor(const SDL_Color color);
		Texture2D* GetTexture2D() const;

	private:
		SDL_Texture* CreateTextTexture();

		SDL_Color m_Color;
		bool m_NeedsUpdate;
		std::string m_Text;
		Font* m_pFont;
		Texture2D* m_pTexture2D;
	};
}
