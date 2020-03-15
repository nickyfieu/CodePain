#include "CodePainPCH.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "Texture2D.h"
#include <SDL.h>

cp::Texture2D::Texture2D(SDL_Texture* texture)
	: BaseComponent{ cp::ComponentType::_Texture2D }
{
	m_pTexture = texture;
	m_pLocalOffset = new Transform();
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

glm::vec3 cp::Texture2D::GetLocalOffset() const
{
	return m_pLocalOffset->GetPosition();
}

SDL_Texture* cp::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}