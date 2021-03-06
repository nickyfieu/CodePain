#include "CodePainPCH.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"

cp::Texture2D::Texture2D(SDL_Texture* texture)
	: BaseComponent{ cp::ComponentType::_Texture2D }
	, m_FlipHorizontal{ false }
{
	m_pTexture = texture;
	int width;
	int height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	m_SrcRect = SDL_Rect{ 0,0,width,height};
	m_DstRect = SDL_FRect{ 0.f,0.f,(float)width,(float)height };
	m_Pivot = { 0,0 };
}

cp::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

void cp::Texture2D::Update(const float)
{
}

void cp::Texture2D::FixedUpdate(const float)
{
}

void cp::Texture2D::DebugDraw() const
{
	Renderer& rendererRef = Renderer::GetInstance();
	if (!rendererRef.gd_RenderTextureTiles)
		return;
	Transform* pTransform = m_pOwner->GetComponent<Transform>(cp::ComponentType::_Transform);
	glm::vec3 pos = pTransform->GetPosition();
	SDL_Rect rect = {0,0,(int)m_DstRect.w,(int)m_DstRect.h};
	rect.x = int(m_DstRect.x + pos.x);
	rect.y = int(m_DstRect.y + pos.y);
	rendererRef.RenderRect(rect, 255, 255, 0);
	
}

void cp::Texture2D::Draw() const
{
	Renderer& rendererRef = Renderer::GetInstance();
	Transform* transform = m_pOwner->GetComponent<Transform>(cp::ComponentType::_Transform);
	SDL_FRect dst = this->GetDstRect();
	SDL_Rect src = this->GetSrcRect();
	// if for whatever reason ptrans == nullptr we have this check
	// it should never be nullptr as the transform component is the first
	// component to be added to the components container
	if (IS_VALID(transform))
	{
		dst.x += transform->GetPosition().x;
		dst.y += transform->GetPosition().y;
	}
	SDL_Rect dstNoFlt{};
	dstNoFlt.x = (int)dst.x; dstNoFlt.y = (int)dst.y; dstNoFlt.w = (int)dst.w; dstNoFlt.h = (int)dst.h;
	
	if (m_FlipHorizontal)
		rendererRef.RenderTexture(m_pTexture, src, dstNoFlt, 0.0, m_Pivot, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
	else
		rendererRef.RenderTexture(m_pTexture, src, dstNoFlt, 0.0, m_Pivot, SDL_RendererFlip::SDL_FLIP_NONE);
}

void cp::Texture2D::AddLocalOffset(const float x,const float y)
{
	m_DstRect.x += x;
	m_DstRect.y += y;
}

void cp::Texture2D::SetLocalScale(float w, float h)
{
	m_DstRect.w = w;
	m_DstRect.h = h;
}

void cp::Texture2D::FlipTexture(bool flip)
{
	m_FlipHorizontal = flip;
}

void cp::Texture2D::SetDstRect(const SDL_FRect& newDstRect)
{
	m_DstRect = newDstRect;
}

void cp::Texture2D::SetSrcRect(const SDL_Rect& newSrcRect)
{
	m_SrcRect = newSrcRect;
}

void cp::Texture2D::SetPivot(const SDL_Point& newPivot)
{
	m_Pivot = newPivot;
}

const SDL_FRect& cp::Texture2D::GetDstRect() const
{
	return m_DstRect;
}

const SDL_Rect& cp::Texture2D::GetSrcRect() const
{
	return m_SrcRect;
}

const SDL_Point& cp::Texture2D::GetPivot() const
{
	return m_Pivot;
}
