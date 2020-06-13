#include "CodePainPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "Components.h"
#include "Observer.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Logger.h"
#include "State.h"
#include <iomanip>

void cp::GameObject::InitializeState(State* startingState)
{
	if (m_pCurrentState != nullptr)
		return;

	m_pCurrentState = startingState;
}

void cp::GameObject::UpdateState()
{
	if (m_pCurrentState != nullptr)
	{
		if (m_pCurrentState->UpdateState(this))
		{
			SAFE_DELETE(m_pCurrentState);
			m_pCurrentState = m_pNewState;
			m_pNewState = nullptr;
		}
	}
}

void cp::GameObject::Update(const float elapsedSec)
{
	if (m_InactiveTimer > 0.f)
	{
		m_InactiveTimer -= elapsedSec;
		return;
	}

	for (size_t i = 0; i < m_AmountOfComponents; i++)
		m_pComponents[i]->Update(elapsedSec);
}

void cp::GameObject::FixedUpdate(float elapsedSec)
{
	if (m_InactiveTimer > 0.f)
		return;

	size_t amountOfRigidBodies = 0;
	std::vector<RigidBody*> m_RigidBodies;
	size_t amountOfCollisionBoxes = 0;
	std::vector<CollisionBox*> m_CollisionBox;

	for (size_t i = 0; i < m_AmountOfComponents; i++)
	{
		if (m_pComponents[i]->GetComponentType() == cp::ComponentType::_RigidBody)
		{
			m_RigidBodies.push_back(static_cast<RigidBody*>(m_pComponents[i]));
			amountOfRigidBodies++;
		}
		else if (m_pComponents[i]->GetComponentType() == cp::ComponentType::_CollisionBox)
		{
			m_CollisionBox.push_back(static_cast<CollisionBox*>(m_pComponents[i]));
			amountOfCollisionBoxes++;
		}
		else
			m_pComponents[i]->FixedUpdate(elapsedSec);
	}

	for (size_t i = 0; i < amountOfRigidBodies; i++)
		m_RigidBodies[i]->PreFixedUpdate(elapsedSec);

	for (size_t i = 0; i < amountOfCollisionBoxes; i++)
		m_CollisionBox[i]->FixedUpdate(elapsedSec);

	for (size_t i = 0; i < amountOfRigidBodies; i++)
		m_RigidBodies[i]->FixedUpdate(elapsedSec);
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
	, m_pCurrentState{ nullptr }
	, m_pNewState{ nullptr }
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

	for (Observer* observer : m_pObservers)
	{
		SAFE_DELETE(observer);
	}

	SAFE_DELETE(m_pCurrentState);
	SAFE_DELETE(m_pNewState);
}

void cp::GameObject::SetActive(bool active)
{
	m_IsActive = active;
}

void cp::GameObject::AddObserver(Observer* observerToAdd)
{
	m_AmountOfObservers++;
	m_pObservers.push_back(observerToAdd);
}

void cp::GameObject::RemoveObserver(Observer* observerToRemove)
{
	auto it = std::find(m_pObservers.cbegin(), m_pObservers.cend(), observerToRemove);
	if (it != m_pObservers.cend())
	{
		m_AmountOfObservers--;
		m_pObservers.erase(it);
	}
}

void cp::GameObject::NotifyObservers(Event event) const
{
	for (size_t i = 0; i < m_AmountOfObservers; i++)
	{
		m_pObservers[i]->OnNotify(this, event);
	}
}

void cp::GameObject::SetNewState(State* newState)
{
	m_pNewState = newState;
}

void cp::GameObject::SetInactiveTimer(float inactivityTIme)
{
	m_InactiveTimer = inactivityTIme;
}
