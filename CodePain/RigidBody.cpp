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

void cp::RigidBody::Update(float)
{
}

void cp::RigidBody::FixedUpdate(float elapsedSec)
{
	Transform* m_pTransform = this->m_pOwner->GetComponent<Transform>(cp::ComponentType::_Transform);

	if (m_ApplyGravity)
		m_Force += m_Gravity;

	if (!m_IsColLeft && m_Force.x < -FLT_EPSILON)
		m_pTransform->Translate(m_Force.x * elapsedSec, 0.f, 0.f);
	else if (!m_IsColRight && m_Force.x > FLT_EPSILON)
		m_pTransform->Translate(m_Force.x * elapsedSec, 0.f, 0.f);

	if (!m_IsColDown && m_Force.y < -FLT_EPSILON)
		m_pTransform->Translate(0.f, m_Force.y * elapsedSec, 0.f);
	else if (!m_IsColUp && m_Force.y > FLT_EPSILON)
		m_pTransform->Translate(0.f, m_Force.y * elapsedSec, 0.f);

	if (m_ResetForceEachFrame)
	{
		m_Force = { 0.f,0.f };
	}
	else
	{
		if (m_IsColDown || m_IsColUp)
			m_Force.y = 0.f;

		if (m_IsColLeft || m_IsColRight)
			m_Force.x = 0.f;
	}

	m_IsColLeft = false;
	m_IsColDown = false;
	m_IsColRight = false;
	m_IsColUp = false;
	m_IsOnGround = false;
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

// note disabling this might cause issues as 
// the force might become so great that you go trough boxes
// as of currently no sweeping aabb implemented yet
void cp::RigidBody::SetResetForceEachFrame(bool use)
{
	m_ResetForceEachFrame = use;
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
