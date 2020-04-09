#include "CodePainPCH.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "Texture2D.h"

cp::Texture2D::Texture2D(SDL_Texture* texture)
	: BaseComponent{ cp::ComponentType::_Texture2D }
{
	m_pTexture = texture;
	m_pLocalOffset = new Transform();
	int width;
	int height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	m_SrcRect = SDL_Rect{ 0,0,width,height};
	m_DstRect = SDL_FRect{ 0.f,0.f,(float)width,(float)height };
}

cp::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
	SAFE_DELETE(m_pLocalOffset);
}

void cp::Texture2D::Update(const float)
{
}

void cp::Texture2D::SetLocalOffset(const float x,const float y,const float z)
{
	m_pLocalOffset->SetPosition(x, y, z);
}

void cp::Texture2D::SetDstRect(const SDL_FRect& newDstRect)
{
	m_DstRect = newDstRect;
}

void cp::Texture2D::SetSrcRect(const SDL_Rect& newSrcRect)
{
	m_SrcRect = newSrcRect;
}

const glm::vec3& cp::Texture2D::GetLocalOffset() const
{
	return m_pLocalOffset->GetPosition();
}

const SDL_FRect& cp::Texture2D::GetDstRect() const
{
	return m_DstRect;
}

const SDL_Rect& cp::Texture2D::GetSrcRect() const
{
	return m_SrcRect;
}
