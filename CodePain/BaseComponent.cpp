#include "CodePainPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"

cp::BaseComponent::BaseComponent(cp::ComponentType type)
	: m_ComponentType{ type }
{

}


void cp::BaseComponent::SetOwner(GameObject* pOwner)
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->RemoveComponent(this);
	}
	m_pOwner = pOwner;
}
