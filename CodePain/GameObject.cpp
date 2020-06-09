#include "CodePainPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Logger.h"
#include <iomanip>

void cp::GameObject::Update(const float elapsedSec)
{
	for (size_t i = 0; i < m_AmountOfComponents; i++)
		m_pComponents[i]->Update(elapsedSec);
}

void cp::GameObject::FixedUpdate(float elapsedSec)
{
	for (size_t i = 0; i < m_AmountOfComponents; i++)
		m_pComponents[i]->FixedUpdate(elapsedSec);
}

void cp::GameObject::Render() const
{
	for (size_t i = 0; i < m_AmountOfComponents; i++)
	{
		m_pComponents[i]->Draw();
#if defined(_DEBUG)
		m_pComponents[i]->DebugDraw();
#endif
	}
}

void cp::GameObject::RemoveComponent(BaseComponent* pToRemove)
{
	if (pToRemove->GetComponentType() == ComponentType::_Transform)
		return Logger::GetInstance().Log(cp::LogLevel::Warning, "GameObject::RemoveComponent trying to remove transform component!");

	m_pComponents.erase(std::find(m_pComponents.cbegin(), m_pComponents.cend(),pToRemove));
	m_AmountOfComponents--;
}

void cp::GameObject::AddComponent(BaseComponent* pToAdd)
{
	if ((pToAdd->GetComponentType() == ComponentType::_Transform) && HasComponent<Transform>(ComponentType::_Transform))
		return Logger::GetInstance().Log(cp::LogLevel::Warning, "GameObject::AddComponent trying to add second transform component!");

	m_pComponents.push_back(pToAdd);
	m_AmountOfComponents++;
	pToAdd->SetOwner(this);
}

cp::GameObject::GameObject(GameObjectType type)
	: m_Type{type}
{
	// Forces Transform Component to be the first component so it will always be found before anything else
	// There will also only be 1 transform component per gameobject never more ( unless there are local transform components in other components )
	AddComponent(new Transform());
}

cp::GameObject::~GameObject()
{
	for (BaseComponent* components : m_pComponents)
	{
		SAFE_DELETE(components);
	}
}

void cp::GameObject::SetActive(bool active)
{
	m_IsActive = active;
}

void cp::GameObject::SetCollisionCallback(CollisionCallback callback)
{
	m_CollisionCallback = callback;
}

void cp::GameObject::OnCollisionCallback(GameObject* self, GameObject* other, CollisionBox::CollisionSide side)
{
	if (m_CollisionCallback)
		m_CollisionCallback(self, other, side);
}
