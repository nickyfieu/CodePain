#include "CodePainPCH.h"
#include "CollisionBox.h"

cp::CollisionBox::CollisionBox(int x, int y, int width, int height, CollisionSide side)
	: BaseComponent(ComponentType::_CollisionBox)
	, m_CollisionSide{side}
	, m_CollisionBox{x,y,width,height}
{
}

void cp::CollisionBox::Update(float)
{
}
