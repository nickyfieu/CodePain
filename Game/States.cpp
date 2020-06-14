#include "CodePainPCH.h"
#include "States.h"
#include "GameObject.h"
#include "Observer.h"

bool Game::EnemyMovingState1::UpdateState(const cp::GameObject* gameObject)
{
	cp::RigidBody* rigidBody = gameObject->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	cp::SpriteComponent* spriteComp = gameObject->GetComponent<cp::SpriteComponent>(cp::ComponentType::_SpriteComponent);

	unsigned int CurrentAnim = spriteComp->GetCurrentAnim();
	if (CurrentAnim != m_LeftAnimName && CurrentAnim != m_RightAnimName)
	{
		spriteComp->SetAnimation("WalkLeft");
	}

	// check if in bubble

	if (rigidBody->GetOnGround())
	{

		if (CurrentAnim == m_LeftAnimName)
		{
			if (rigidBody->GetColLeft())
			{
				spriteComp->SetAnimation("WalkRight");
				glm::vec2 currentForce = rigidBody->GetForce();
				rigidBody->SetForce({ 100.f, currentForce.y });
			}
			else
			{
				glm::vec2 currentForce = rigidBody->GetForce();
				rigidBody->SetForce({ -100.f, currentForce.y });
			}
		}
		else if (CurrentAnim == m_RightAnimName)
		{
			if (rigidBody->GetColRight())
			{
				spriteComp->SetAnimation("WalkLeft");
				glm::vec2 currentForce = rigidBody->GetForce();
				rigidBody->SetForce({ -100.f, currentForce.y });
			}
			else
			{
				glm::vec2 currentForce = rigidBody->GetForce();
				rigidBody->SetForce({ 100.f, currentForce.y });
			}
		}

		if ((rand() % 960) == 240)
		{
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ 0.f, 325.f });
		}

	}
	
	return false;
}

bool Game::EnemyMovingState2::UpdateState(const cp::GameObject* gameObject)
{
	cp::RigidBody* rigidBody = gameObject->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	cp::SpriteComponent* spriteComp = gameObject->GetComponent<cp::SpriteComponent>(cp::ComponentType::_SpriteComponent);

	unsigned int CurrentAnim = spriteComp->GetCurrentAnim();
	if (CurrentAnim != m_LeftAnimName && CurrentAnim != m_RightAnimName)
	{
		spriteComp->SetAnimation("WalkLeft");
	}

	if (rigidBody->GetOnGround())
	{

		if (CurrentAnim == m_LeftAnimName)
		{
			if (rigidBody->GetColLeft())
			{
				spriteComp->SetAnimation("WalkRight");
				glm::vec2 currentForce = rigidBody->GetForce();
				rigidBody->SetForce({ 100.f, currentForce.y });
			}
			else
			{
				glm::vec2 currentForce = rigidBody->GetForce();
				rigidBody->SetForce({ -100.f, currentForce.y });
			}
		}
		else if (CurrentAnim == m_RightAnimName)
		{
			if (rigidBody->GetColRight())
			{
				spriteComp->SetAnimation("WalkLeft");
				glm::vec2 currentForce = rigidBody->GetForce();
				rigidBody->SetForce({ -100.f, currentForce.y });
			}
			else
			{
				glm::vec2 currentForce = rigidBody->GetForce();
				rigidBody->SetForce({ 100.f, currentForce.y });
			}
		}
	}

	return false;
}

bool Game::EnemyMovingState3::UpdateState(const cp::GameObject* gameObject)
{
	cp::RigidBody* rigidBody = gameObject->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	cp::SpriteComponent* spriteComp = gameObject->GetComponent<cp::SpriteComponent>(cp::ComponentType::_SpriteComponent);

	unsigned int CurrentAnim = spriteComp->GetCurrentAnim();
	if (CurrentAnim != m_LeftAnimName && CurrentAnim != m_RightAnimName)
	{
		spriteComp->SetAnimation("WalkLeft");
	}

	// check if in bubble

	if (rigidBody->GetOnGround())
	{
		glm::vec2 currentForce = rigidBody->GetForce();
		rigidBody->SetForce({ currentForce.x, 180.f });
		m_HasHitGround = true;
	}

	if ((CurrentAnim == m_LeftAnimName) && m_HasHitGround)
	{
		if (rigidBody->GetColLeft())
		{
			spriteComp->SetAnimation("WalkRight");
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ 240.f, currentForce.y });
		}
		else
		{
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ -240.f, currentForce.y });
		}
	}
	else if ((CurrentAnim == m_RightAnimName) && m_HasHitGround)
	{
		if (rigidBody->GetColRight())
		{
			spriteComp->SetAnimation("WalkLeft");
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ -240.f, currentForce.y });
		}
		else
		{
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ 240.f, currentForce.y });
		}
	}

	return false;
}

Game::EnemyMovingState4::EnemyMovingState4(bool isMovingDown, float horizontalForce, float verticalForce)
	: m_IsMovingDown{ isMovingDown }
	, m_HorizontalForce{ horizontalForce }
	, m_VerticalForce{ verticalForce }
{
}

bool Game::EnemyMovingState4::UpdateState(const cp::GameObject* gameObject)
{
	cp::RigidBody* rigidBody = gameObject->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	cp::SpriteComponent* spriteComp = gameObject->GetComponent<cp::SpriteComponent>(cp::ComponentType::_SpriteComponent);

	unsigned int CurrentAnim = spriteComp->GetCurrentAnim();
	if (CurrentAnim != m_LeftAnimName && CurrentAnim != m_RightAnimName)
	{
		spriteComp->SetAnimation("WalkLeft");
	}

	// check if in bubble

	if (m_IsMovingDown)
	{
		if (rigidBody->GetColUp())
		{
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ currentForce.x, -m_VerticalForce });
			m_IsMovingDown = false;
		}
		else
		{
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ currentForce.x, m_VerticalForce });
		}
	}
	else
	{
		if (rigidBody->GetOnGround())
		{
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ currentForce.x, m_VerticalForce });
			m_IsMovingDown = true;
		}
		else
		{
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ currentForce.x, -m_VerticalForce });
		}
	}

	if ((CurrentAnim == m_LeftAnimName))
	{
		if (rigidBody->GetColLeft())
		{
			spriteComp->SetAnimation("WalkRight");
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ m_HorizontalForce, currentForce.y });
		}
		else
		{
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ -m_HorizontalForce, currentForce.y });
		}
	}
	else if ((CurrentAnim == m_RightAnimName))
	{
		if (rigidBody->GetColRight())
		{
			spriteComp->SetAnimation("WalkLeft");
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ -m_HorizontalForce, currentForce.y });
		}
		else
		{
			glm::vec2 currentForce = rigidBody->GetForce();
			rigidBody->SetForce({ m_HorizontalForce, currentForce.y });
		}
	}

	return false;
}

bool Game::PlayerMovingState::UpdateState(const cp::GameObject* gameObject)
{
	UNREFERENCED_PARAMETER(gameObject);

	return false;
}
