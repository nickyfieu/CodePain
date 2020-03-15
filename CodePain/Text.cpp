#include "CodePainPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "Text.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

cp::Text::Text(const std::string& text, Font* font, SDL_Color color)
	: BaseComponent(cp::ComponentType::_Text)
	, m_NeedsUpdate(true)
	, m_Text(text)
	, m_pFont(font)
	, m_pTexture2D(nullptr)
	, m_Color{ color }
{ 
	m_pTransform = new Transform();
}

cp::Text::~Text()
{
	SAFE_DELETE(m_pTexture2D);
	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pTransform);
}

void cp::Text::Update(const float)
{
	if (m_NeedsUpdate)
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
		SAFE_DELETE(m_pTexture2D);
		m_pTexture2D = new Texture2D(texture);
		m_NeedsUpdate = false;
	}
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

void cp::Text::SetLocalOffset(const float x, const float y, const float z)
{
	m_pTransform->SetPosition(x, y, z);
}

glm::vec3 cp::Text::GetLocalOffset() const
{
	return m_pTransform->GetPosition();
}

cp::Texture2D* cp::Text::GetTexture2D() const
{
	return m_pTexture2D;
}
