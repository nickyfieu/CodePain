#include "CodePainPCH.h"
#include "Commands.h"
#include "GameObject.h"
#include "Logger.h"
#include "Components.h"
#include "InputManager.h"
#include "AudioLocator.h"
#include "BubbleBobbleAudio.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputHandler.h"
#include "Observers.h"
#include "Scene.h"
#include "GameObject.h"
#include "States.h"

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

void Game::StartP1Game::Execute(const cp::GameObject*)
{
	if (cp::GameManager::GetInstance().GetState() != cp::GameManager::GameState::Menu)
		return;

	cp::SceneManager& sceneManager = cp::SceneManager::GetInstance();
	cp::Scene* activeScene = sceneManager.GetActiveScene();

#pragma region P1TestObj

	{
		cp::GameObject* p1 = activeScene->GetAllGameObjectsOfType(cp::GameObjectType::Player1)[0];
		p1->InitializeState(new Game::IdleState());

		// sprite component
		cp::SpriteComponent* sprite = new cp::SpriteComponent(cp::ResourceManager::GetInstance().LoadSDLTexture("CharacterData/HeroGreen_Enrique.png"));
		cp::Texture2D* texture = sprite->GetTextureComponent();
		texture->AddLocalOffset(0.f, -40.f);
		texture->SetLocalScale(40.f, 40.f);

		cp::Sprite anim = cp::Sprite{};
		anim.endOnFirstFrame = false;
		anim.flipTexture = false;
		anim.frameTime = 0.25f;
		anim.nrOfLoops = 0;
		anim.spritesX = 4;
		anim.spritesY = 1;
		anim.spriteWidth = 16;
		anim.spriteHeight = 16;
		anim.xOffset = 0;
		anim.yOffset = 0;
		sprite->AddAnimation("WalkRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("WalkLeft", anim);
		anim.spritesX = 1;
		sprite->AddAnimation("IdleLeft", anim);
		anim.flipTexture = false;
		sprite->AddAnimation("IdleRight", anim);
		anim.spritesX = 4;
		anim.yOffset = 16;
		anim.frameTime = 0.1f;
		sprite->AddAnimation("DieRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("DieLeft", anim);
		anim.frameTime = 0.5f;
		anim.flipTexture = false;
		anim.yOffset = 32;
		anim.spritesX = 2;
		sprite->AddAnimation("ShootRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("ShootLeft", anim);
		sprite->SetAnimation("IdleRight");
		sprite->UnPause();
		p1->AddComponent(sprite);
		texture->SetOwner(p1);

		// collision
		p1->AddComponent(new cp::CollisionBox(31, -18, 3, 15, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::CollisionBox(6, -18, 3, 15, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::CollisionBox(6, -3, 28, 3, cp::CollisionBox::CollisionSide::down, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::CollisionBox(9, -15, 22, 12, cp::CollisionBox::CollisionSide::all, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::RigidBody(true));

		// translation
		p1->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->Translate(60, -480, 0);

		// score
		p1->AddComponent(new cp::Score(0));

		// health
		cp::Health* healthComp = new cp::Health(false, 5);
		p1->AddComponent(healthComp);


		cp::Command* jmpCommand = new Game::JumpCommand();
		cp::Command* lftCommand = new Game::LeftCommand();
		cp::Command* rhtCommand = new Game::RightCommand();
		cp::Command* fireCommand = new Game::FireCommand();
		cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::KeyW, cp::InputState::Hold, jmpCommand);
		cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::KeyA, cp::InputState::Hold, lftCommand);
		cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::KeyD, cp::InputState::Hold, rhtCommand);
		cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::LeftCtrl, cp::InputState::Pressed, fireCommand);

		// notify
		p1->AddObserver(new Game::IdleEvent());
		p1->AddObserver(new Game::TopBottomLevelCollision());
		p1->AddObserver(new Game::PlayerHealthChange());
		
		healthComp->AddHealth(0);
	}

#pragma endregion

	cp::GameManager::GetInstance().SetGameState(cp::GameManager::GameState::Game);
}

void Game::StartP2Game::Execute(const cp::GameObject* actor)
{
	if (cp::GameManager::GetInstance().GetState() != cp::GameManager::GameState::Menu)
		return;

	cp::SceneManager& sceneManager = cp::SceneManager::GetInstance();
	cp::Scene* activeScene = sceneManager.GetActiveScene();

#pragma region P1TestObj
	{
		cp::GameObject* p1 = activeScene->GetAllGameObjectsOfType(cp::GameObjectType::Player1)[0];
		p1->InitializeState(new Game::IdleState());

		// sprite component
		cp::SpriteComponent* sprite = new cp::SpriteComponent(cp::ResourceManager::GetInstance().LoadSDLTexture("CharacterData/HeroGreen_Enrique.png"));
		cp::Texture2D* texture = sprite->GetTextureComponent();
		texture->AddLocalOffset(0.f, -40.f);
		texture->SetLocalScale(40.f, 40.f);

		cp::Sprite anim = cp::Sprite{};
		anim.endOnFirstFrame = false;
		anim.flipTexture = false;
		anim.frameTime = 0.25f;
		anim.nrOfLoops = 0;
		anim.spritesX = 4;
		anim.spritesY = 1;
		anim.spriteWidth = 16;
		anim.spriteHeight = 16;
		anim.xOffset = 0;
		anim.yOffset = 0;
		sprite->AddAnimation("WalkRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("WalkLeft", anim);
		anim.spritesX = 1;
		sprite->AddAnimation("IdleLeft", anim);
		anim.flipTexture = false;
		sprite->AddAnimation("IdleRight", anim);
		anim.spritesX = 4;
		anim.yOffset = 16;
		anim.frameTime = 0.1f;
		sprite->AddAnimation("DieRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("DieLeft", anim);
		anim.frameTime = 0.5f;
		anim.flipTexture = false;
		anim.yOffset = 32;
		anim.spritesX = 2;
		sprite->AddAnimation("ShootRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("ShootLeft", anim);
		sprite->SetAnimation("IdleRight");
		sprite->UnPause();
		p1->AddComponent(sprite);
		texture->SetOwner(p1);

		// collision
		p1->AddComponent(new cp::CollisionBox(31, -18, 3, 15, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::CollisionBox(6, -18, 3, 15, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::CollisionBox(6, -3, 28, 3, cp::CollisionBox::CollisionSide::down, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::CollisionBox(9, -15, 22, 12, cp::CollisionBox::CollisionSide::all, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::RigidBody(true));

		// translation
		p1->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->Translate(60, -480, 0);

		// score
		p1->AddComponent(new cp::Score(0));

		// health
		p1->AddComponent(new cp::Health(false, 5));
		
		
		cp::Command* jmpCommand = new Game::JumpCommand();
		cp::Command* lftCommand = new Game::LeftCommand();
		cp::Command* rhtCommand = new Game::RightCommand();
		cp::Command* fireCommand = new Game::FireCommand();
		cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::KeyW, cp::InputState::Hold, jmpCommand);
		cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::KeyA, cp::InputState::Hold, lftCommand);
		cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::KeyD, cp::InputState::Hold, rhtCommand);
		cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::LeftCtrl, cp::InputState::Pressed, fireCommand);

		// notify
		p1->AddObserver(new Game::IdleEvent());
		p1->AddObserver(new Game::TopBottomLevelCollision());

	}
#pragma endregion

#pragma region P2TestObj
	{

		cp::GameObject* p2 = activeScene->GetAllGameObjectsOfType(cp::GameObjectType::Player2)[0];
		p2->InitializeState(new Game::IdleState());
		// sprite component
		cp::SpriteComponent* sprite = new cp::SpriteComponent(cp::ResourceManager::GetInstance().LoadSDLTexture("CharacterData/HeroBlue_Enrique.png"));
		cp::Texture2D* texture = sprite->GetTextureComponent();
		texture->AddLocalOffset(0.f, -40.f);
		texture->SetLocalScale(40.f, 40.f);

		cp::Sprite anim = cp::Sprite{};
		anim.endOnFirstFrame = false;
		anim.flipTexture = false;
		anim.frameTime = 0.25f;
		anim.nrOfLoops = 0;
		anim.spritesX = 4;
		anim.spritesY = 1;
		anim.spriteWidth = 16;
		anim.spriteHeight = 16;
		anim.xOffset = 0;
		anim.yOffset = 0;
		sprite->AddAnimation("WalkRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("WalkLeft", anim);
		anim.spritesX = 1;
		sprite->AddAnimation("IdleLeft", anim);
		anim.flipTexture = false;
		sprite->AddAnimation("IdleRight", anim);
		anim.yOffset = 16;
		anim.spritesX = 4;
		anim.frameTime = 0.1f;
		sprite->AddAnimation("DieRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("DieLeft", anim);
		anim.frameTime = 0.5f;
		anim.flipTexture = false;
		anim.yOffset = 32;
		anim.spritesX = 2;
		sprite->AddAnimation("ShootRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("ShootLeft", anim);
		sprite->SetAnimation("IdleLeft");
		sprite->UnPause();
		p2->AddComponent(sprite);
		texture->SetOwner(p2);

		// collision
		p2->AddComponent(new cp::CollisionBox(35, -18, 3, 15, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_dynamic));
		p2->AddComponent(new cp::CollisionBox(3, -18, 3, 15, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_dynamic));
		p2->AddComponent(new cp::CollisionBox(3, -3, 34, 3, cp::CollisionBox::CollisionSide::down, cp::CollisionBox::CollisionType::_dynamic));
		p2->AddComponent(new cp::CollisionBox(9, -15, 22, 12, cp::CollisionBox::CollisionSide::all, cp::CollisionBox::CollisionType::_dynamic));
		p2->AddComponent(new cp::RigidBody(true));

		// score
		p2->AddComponent(new cp::Score(0));

		// health
		p2->AddComponent(new cp::Health(false, 5));

		// translation
		p2->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->Translate(540, -480, 0);

		cp::Command* controllerJmpCommand = new Game::ControllerJumpCommand();
		cp::Command* controllerHrzCommand = new Game::ControllerHorizontalCommand();
		cp::Command* controllerFireCommand = new Game::ControllerFIreCommand();
		cp::InputHandler::GetInstance().AddInput(cp::ControllerButton::ButtonA, cp::InputState::Pressed, controllerJmpCommand);
		cp::InputHandler::GetInstance().AddInput(cp::ControllerAxis::LeftThumbStickX, controllerHrzCommand);
		cp::InputHandler::GetInstance().AddInput(cp::ControllerAxis::RightTrigger, controllerFireCommand);

		// notify
		p2->AddObserver(new Game::IdleEvent());
		p2->AddObserver(new Game::TopBottomLevelCollision());
	}

#pragma endregion

	cp::GameManager::GetInstance().SetGameState(cp::GameManager::GameState::Game);
}

void Game::Start1v1Game::Execute(const cp::GameObject* actor)
{
	if (cp::GameManager::GetInstance().GetState() != cp::GameManager::GameState::Menu)
		return;


	cp::GameManager::GetInstance().SetGameState(cp::GameManager::GameState::Game);
}
