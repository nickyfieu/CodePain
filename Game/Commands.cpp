#include "CodePainPCH.h"
#include "Commands.h"
#include "GameObject.h"
#include "Logger.h"
#include "Components.h"
#include "InputManager.h"
#include "AudioLocator.h"
#include "BubbleBobbleAudio.h"

void Game::JumpCommand::Execute(const cp::GameObject* actor)
{
	cp::RigidBody* rigidBody = actor->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	if (rigidBody == nullptr)
		return;

	glm::vec2 force = rigidBody->GetForce();
	if (rigidBody->GetOnGround() && (rigidBody->GetForce().y <= FLT_EPSILON))
	{
		rigidBody->SetForce({ force.x,325.f });
		cp::AudioLocator::GetAudio()->PlaySfx((int)Game::BubbleBobbleAudio::Sfx::Jump);
	}
}



void Game::LeftCommand::Execute(const cp::GameObject* actor)
{
	cp::RigidBody* rigidBody = actor->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	if (rigidBody == nullptr)
		return;

	glm::vec2 force = rigidBody->GetForce();
	rigidBody->SetForce({ -75.f,force.y });
}

void Game::RightCommand::Execute(const cp::GameObject* actor)
{
	cp::RigidBody* rigidBody = actor->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	if (rigidBody == nullptr)
		return;

	glm::vec2 force = rigidBody->GetForce();
	rigidBody->SetForce({ 75.f,force.y });
}

void Game::FireCommand::Execute(const cp::GameObject*)
{
	cp::Logger::GetInstance().Log(cp::LogLevel::Info, "Pew");
}

void Game::ControllerJumpCommand::Execute(const cp::GameObject* actor)
{
	cp::RigidBody* rigidBody = actor->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	if (rigidBody == nullptr)
		return;

	glm::vec2 force = rigidBody->GetForce();
	if (rigidBody->GetOnGround() && (rigidBody->GetForce().y <= FLT_EPSILON))
	{
		rigidBody->SetForce({ force.x,325.f });
		cp::AudioLocator::GetAudio()->PlaySfx((int)Game::BubbleBobbleAudio::Sfx::Jump);
	}
}

// this command assumes to be used on the left thumb stick
void Game::ControllerHorizontalCommand::Execute(const cp::GameObject* actor)
{
	cp::RigidBody* rigidBody = actor->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	if (rigidBody == nullptr)
		return;

	cp::InputManager& inputManager = cp::InputManager::GetInstance();
	float axisValue = inputManager.GetControllerAxisValue(cp::ControllerAxis::LeftThumbStickX, (int)actor->GetType() - (int)cp::GameObjectType::Player2);

	
	glm::vec2 force = rigidBody->GetForce();
	if (axisValue < -FLT_EPSILON)
	{
		rigidBody->SetForce({ -100.f,force.y });
	}
	else if (axisValue > FLT_EPSILON)
	{
		rigidBody->SetForce({ 100.f,force.y });
	}
}

// this command assumes to be used on the right trigger
void Game::ControllerFIreCommand::Execute(const cp::GameObject* actor)
{
	cp::InputManager& inputManager = cp::InputManager::GetInstance();
	float AxisValue = inputManager.GetControllerAxisValue(cp::ControllerAxis::RightTrigger, (int)actor->GetType() - (int)cp::GameObjectType::Player2);
	float prevAxisValue = inputManager.GetPreviousControllerAxisValue(cp::ControllerAxis::RightTrigger, (int)actor->GetType() - (int)cp::GameObjectType::Player2);

	if (!(prevAxisValue > 0.f) && (AxisValue > 0.f))
	{
		cp::Logger::GetInstance().Log(cp::LogLevel::Info, "Pew Pew");
	}
}
