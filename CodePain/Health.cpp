#include "CodePainPCH.h"
#include "Health.h"
#include "GameObject.h"
#include "Observer.h"

bool cp::Health::IsDead() const
{
	return m_CurrentHealth <= 0;
}

void cp::Health::AddHealth(int amount)
{
	if (m_GotHit && (amount < 0))
		return;

	if ((m_GotHit == false) && (amount < 0))
	{
		m_GotHit = true;
		m_ImunityTimer = 0.f;
	}

	m_CurrentHealth += amount;

	if (m_CurrentHealth > m_MaxHealth)
		m_CurrentHealth = m_MaxHealth;

	if (m_CurrentHealth < 0)
	{
		m_CurrentHealth = 0;
		if (m_pOwner && m_DeleteOnDeath)
			m_pOwner->SetDestroy(true);
	}

	if (m_pOwner)
		m_pOwner->NotifyObservers(cp::Event::EVENT_HP_CHANGE);
}

cp::Health::Health(bool deleteObjOnDeath, int maxHealth)
	: cp::BaseComponent(cp::ComponentType::_Health)
	, m_DeleteOnDeath{deleteObjOnDeath}
	, m_MaxHealth{maxHealth}
	, m_CurrentHealth{ maxHealth }
{
}

void cp::Health::Update(float elapsedSec)
{
	m_ImunityTimer += elapsedSec;
	if (m_ImunityTimer >= m_ImunityTime)
	{
		m_GotHit = false;
	}
}

void cp::Health::FixedUpdate(float)
{
}

void cp::Health::Draw() const
{
}

void cp::Health::DebugDraw() const
{
}
