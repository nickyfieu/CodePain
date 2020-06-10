#include "CodePainPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "Text.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

#include "FrameRate.h"
#include "GameObject.h"

cp::Text::Text(const std::string& text, Font* font, SDL_Color color)
	: BaseComponent(cp::ComponentType::_Text)
	, m_NeedsUpdate(true)
	, m_Text(text)
	, m_pFont(font)
	, m_pTexture2D(nullptr)
	, m_Color{ color }
{ 
	m_pTexture2D = new Texture2D(CreateTextTexture());
	m_pTexture2D->SetOwner(this->m_pOwner);
}

cp::Text::~Text()
{
	SAFE_DELETE(m_pTexture2D);
	SAFE_DELETE(m_pFont);
}

void cp::Text::Update(const float)
{

	if (m_NeedsUpdate)
	{
		SDL_FRect dst = m_pTexture2D->GetDstRect();
		SAFE_DELETE(m_pTexture2D);
		m_pTexture2D = new Texture2D(CreateTextTexture());
		m_pTexture2D->AddLocalOffset(dst.x, dst.y);
		m_pTexture2D->SetOwner(this->m_pOwner);
		m_NeedsUpdate = false;
	}
}

void cp::Text::FixedUpdate(const float)
{
	// only updates once a second so no need to put it in the normal update
	FrameRate* pFrameRate = m_pOwner->GetComponent<FrameRate>(cp::ComponentType::_FrameRate);
	if (IS_VALID(pFrameRate))
	{
		float fps = 0.f;
		if (pFrameRate->GetFrameRate(fps))
		{
			this->SetText(std::to_string(int(fps)) + " FPS");
		}

	}
}

void cp::Text::DebugDraw() const
{
}

void cp::Text::Draw() const
{
	m_pTexture2D->Draw();
}

void cp::Text::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void cp::Text::SetTextColor(const SDL_Color color)
{
	m_Color = color;
	m_NeedsUpdate = true;
}

cp::Texture2D* cp::Text::GetTexture2D() const
{
	return m_pTexture2D;
}

SDL_Texture* cp::Text::CreateTextTexture()
{
	SDL_Surface* surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	return texture;
}
