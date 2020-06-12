#include "CodePainPCH.h"
#include "Commands.h"
#include "GameObject.h"
#include "Logger.h"
#include "Components.h"
#include "InputManager.h"

void Game::JumpCommand::Execute(const cp::GameObject* actor)
{
	cp::RigidBody* rigidBody = actor->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	if (rigidBody == nullptr)
		return;

	if (rigidBody->GetOnGround())
	{
		glm::vec2 force = rigidBody->GetForce();
		rigidBody->SetForce({ force.x,325.f });
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

	if (rigidBody->GetOnGround())
	{
		glm::vec2 force = rigidBody->GetForce();
		rigidBody->SetForce({ force.x,325.f });
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
		rigidBody->SetForce({ -75.f,force.y });
	else if (axisValue > -FLT_EPSILON)
		rigidBody->SetForce({ 75.f,force.y });
}
