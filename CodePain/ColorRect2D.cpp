#include "CodePainPCH.h"
#include "ColorRect2D.h"

#include "Renderer.h"
#include "GameObject.h"

cp::ColorRect2D::ColorRect2D(int x, int y, int width, int height, Uint32 rgba)
	: BaseComponent(ComponentType::_ColorRect2D)
	, m_ColorBox{ SDL_Rect{x,y,width,height} }
	, m_RGBA{rgba}
{
}

void cp::ColorRect2D::Update(const float)
{
}

void cp::ColorRect2D::FixedUpdate(const float)
{
}

void cp::ColorRect2D::DebugDraw() const
{
}

void cp::ColorRect2D::Draw() const
{
	Renderer& rendererRef = Renderer::GetInstance();
	Transform* pTransform = m_pOwner->GetComponent<Transform>(cp::ComponentType::_Transform);
	ColorRect2D* pColRect2D = m_pOwner->GetComponent<ColorRect2D>(cp::ComponentType::_ColorRect2D);
	SDL_Rect box = m_ColorBox;
	// if for whatever reason ptrans == nullptr we have this check
	// it should never be nullptr as the transform component is the first
	// component to be added to the components container
	if (IS_VALID(pTransform))
	{
		box.x += (int)pTransform->GetPosition().x;
		box.y += (int)pTransform->GetPosition().y;
	}

	Uint32 rgba = pColRect2D->GetRGBA();
	Uint8 r = Uint8(rgba >> 0);
	Uint8 g = Uint8(rgba >> 8);
	Uint8 b = Uint8(rgba >> 16);
	Uint8 a = Uint8(rgba >> 24);

	rendererRef.RenderCollorRect(box, r, g, b, a);
}
