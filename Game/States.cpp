#include "CodePainPCH.h"
#include "States.h"
#include "GameObject.h"
#include "Observer.h"

bool Game::IdleState::UpdateState(const cp::GameObject* gameObject)
{
	gameObject->NotifyObservers(cp::Event::EVENT_IDLE);
	return false;
}

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
