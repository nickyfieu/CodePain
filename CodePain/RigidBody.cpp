#include "CodePainPCH.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "Transform.h"

cp::RigidBody::RigidBody(bool applyGravity)
	: BaseComponent(cp::ComponentType::_RigidBody)
	, m_ApplyGravity{applyGravity}
{
}

cp::RigidBody::~RigidBody()
{
}

void cp::RigidBody::PreFixedUpdate(float elapsedSec)
{
	if ((m_IsColDown && (m_Force.y < -FLT_EPSILON)) || (m_IsColUp && (m_Force.y > FLT_EPSILON)))
		m_Force.y = 0.f;

	if ((m_IsColLeft && (m_Force.x < -FLT_EPSILON)) || (m_IsColRight && (m_Force.x > FLT_EPSILON)))
		m_Force.x = 0.f;

	if (m_ApplyGravity)
		m_Force += m_Gravity * elapsedSec;

	m_Velocity = m_Force * elapsedSec;

	m_EntryUDTime = 1.f;
	m_EntryLRTime = 1.f;
	m_IsColLeft = false;
	m_IsColDown = false;
	m_IsColRight = false;
	m_IsColUp = false;
	m_IsOnGround = false;
}

void cp::RigidBody::Update(float)
{
}

void cp::RigidBody::FixedUpdate(float)
{
	Transform* m_pTransform = this->m_pOwner->GetComponent<Transform>(cp::ComponentType::_Transform);
	
	m_pTransform->Translate(m_Velocity.x * m_EntryLRTime, m_Velocity.y * m_EntryUDTime, 0.f);

	m_Velocity = { 0.f,0.f };

	if (m_ResetXForceEachFrame)
		m_Force.x = 0;
}

void cp::RigidBody::DebugDraw() const
{


}

void cp::RigidBody::AddForce(const glm::vec2& force, const BaseComponent* owner)
{
	auto it = std::find(m_pAddedForce.cbegin(), m_pAddedForce.cend(), owner);
	// force only gets cleared  once evry fixedupdate cycle  so therefore the owner has to be passsed to not have 
	// a single object add a force multiple times troughout a single frame ( frame dependant force )
	if (it == m_pAddedForce.cend()) 
		return;

	m_Force += force;
	m_pAddedForce.push_back(owner);
}

void cp::RigidBody::SetIsColLeft(bool col)
{
	m_IsColLeft = col;
}

void cp::RigidBody::SetIsColRight(bool col)
{
	m_IsColRight = col;
}

void cp::RigidBody::SetIsColDown(bool col)
{
	m_IsColDown = col;
}

void cp::RigidBody::SetIsColUp(bool col)
{
	m_IsColUp = col;
}

void cp::RigidBody::SetIsOnGround(bool col)
{
	m_IsOnGround = col;
}

void cp::RigidBody::SetUseGravit(bool use)
{
	m_ApplyGravity = use;
}

void cp::RigidBody::SetResetXForceEachFrame(bool use)
{
	m_ResetXForceEachFrame = use;
}

void cp::RigidBody::SetEntryUDTime(float time)
{
	if (time < m_EntryUDTime)
		m_EntryUDTime = time;
}

void cp::RigidBody::SetEntryLRTime(float time)
{
	if (time < m_EntryLRTime)
		m_EntryLRTime = time;
}

void cp::RigidBody::SetGravitationalForce(const glm::vec2& gravity)
{
	m_Gravity = gravity;
}

// overwrites any other forces set / added before this
void cp::RigidBody::SetForce(const glm::vec2& force)
{
	m_Force = force;
}

void cp::RigidBody::Draw() const
{
}
