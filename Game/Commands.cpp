#include "CodePainPCH.h"
#include "Commands.h"
#include "GameObject.h"
#include "Logger.h"
#include "Components.h"

void Game::JumpCommand::Execute(const cp::GameObject* actor)
{
	cp::RigidBody* rigidBody = actor->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	if (rigidBody == nullptr)
		return;

	glm::vec2 force = rigidBody->GetForce();
	rigidBody->SetForce({ force.x,1000.f });
}

void Game::LeftCommand::Execute(const cp::GameObject* actor)
{
	cp::RigidBody* rigidBody = actor->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	if (rigidBody == nullptr)
		return;

	glm::vec2 force = rigidBody->GetForce();
	rigidBody->SetForce({ -150.f,force.y });
}

void Game::RightCommand::Execute(const cp::GameObject* actor)
{
	cp::RigidBody* rigidBody = actor->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	if (rigidBody == nullptr)
		return;


	glm::vec2 force = rigidBody->GetForce();
	rigidBody->SetForce({ 150.f,force.y });
}

void Game::FireCommand::Execute(const cp::GameObject*)
{
	cp::Logger::GetInstance().Log(cp::LogLevel::Info, "Pew");
}
