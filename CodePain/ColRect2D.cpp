#include "CodePainPCH.h"
#include "ColRect2D.h"

cp::ColRect2D::ColRect2D(int x, int y, int width, int height, Uint32 rgba)
	: BaseComponent(ComponentType::_ColRect2D)
	, m_ColorBox{ SDL_Rect{x,y,width,height} }
	, m_RGBA{rgba}
{
}

void cp::ColRect2D::Update(float)
{
}
