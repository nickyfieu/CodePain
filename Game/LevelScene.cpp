#include "CodePainPCH.h"
#include "LevelScene.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "BubbleBobbleDataReader.h"
#include "GameObject.h"
#include "Logger.h"
#include "Components.h"
#include "InputHandler.h"
#include "InputCommand.h"
#include "Commands.h"
#include "States.h"
#include "Observers.h"
#include "GameManager.h"

void Game::LevelScene::LoadSceneData() const
{
	cp::Scene* scene = cp::SceneManager::GetInstance().CreateScene("LevelScene");
	BubbleBobbleDataReader& dataReader = BubbleBobbleDataReader::GetInstance();
	dataReader.ReadLevelData(scene, "LevelData/SeperatedLevelData.dat", "LevelData/LevelParallaxColors.png");
	dataReader.ReadEnemyData("CharacterData/SeperatedEnemyData.dat");
	
#pragma region enemySpawner
	{
		cp::GameManager& gameManager = cp::GameManager::GetInstance();
		gameManager.GetManagerObj()->AddObserver(new Game::SpawnLevelEnemies());
	}
#pragma endregion

#pragma region P1TestObj
	{
		cp::GameObject* p1 = new cp::GameObject(cp::GameObjectType::Player1);
		p1->InitializeState(new Game::IdleState());
		scene->Add(p1);

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
		sprite->SetAnimation("DieRight");
		sprite->UnPause();
		p1->AddComponent(sprite);
		texture->SetOwner(p1);

		// collision
		p1->AddComponent(new cp::CollisionBox(31, -18, 3, 15, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::CollisionBox(6, -18, 3, 15, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::CollisionBox(6, -3, 28, 3, cp::CollisionBox::CollisionSide::down, cp::CollisionBox::CollisionType::_dynamic));
		p1->AddComponent(new cp::RigidBody(true));

		// translation
		p1->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->Translate(100, -50, 0);

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

	}
#pragma endregion

#pragma region P2TestObj
	{
		cp::GameObject* p2 = new cp::GameObject(cp::GameObjectType::Player2);
		p2->InitializeState(new Game::IdleState());
		scene->Add(p2);
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
		sprite->SetAnimation("ShootLeft");
		sprite->UnPause();
		p2->AddComponent(sprite);
		texture->SetOwner(p2);

		// collision
		p2->AddComponent(new cp::CollisionBox(35, -18, 3, 15, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_dynamic));
		p2->AddComponent(new cp::CollisionBox(3, -18, 3, 15, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_dynamic));
		p2->AddComponent(new cp::CollisionBox(3, -3, 34, 3, cp::CollisionBox::CollisionSide::down, cp::CollisionBox::CollisionType::_dynamic));
		p2->AddComponent(new cp::RigidBody(true));

		// translation
		p2->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->Translate(100, -50, 0);

		cp::Command* controllerJmpCommand = new Game::ControllerJumpCommand();
		cp::Command* controllerHrzCommand = new Game::ControllerHorizontalCommand();
		cp::Command* controllerFireCommand = new Game::ControllerFIreCommand();
		cp::InputHandler::GetInstance().AddInput(cp::ControllerButton::ButtonA, cp::InputState::Pressed, controllerJmpCommand);
		cp::InputHandler::GetInstance().AddInput(cp::ControllerAxis::LeftThumbStickX, controllerHrzCommand);
		cp::InputHandler::GetInstance().AddInput(cp::ControllerAxis::RightTrigger, controllerFireCommand);

		// notify
		p2->AddObserver(new Game::IdleEvent());
	}

#pragma endregion

#pragma region SideCollision

	std::vector<cp::GameObject*> levelObjects = scene->GetAllGameObjectsOfType(cp::GameObjectType::level);

	for (size_t i = 0; i < levelObjects.size(); i++)
	{
		levelObjects[i]->AddComponent(new cp::CollisionBox(0, -500, 2 * 20, 20 * 25 + 2 * 500, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_static));
		levelObjects[i]->AddComponent(new cp::CollisionBox((20 * 30), -500, 2 * 20, 20 * 25 + 2 * 500, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_static));
	}

#pragma endregion


}
