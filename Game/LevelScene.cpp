#include "CodePainPCH.h"
#include "LevelScene.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "BubbleBobbleLevelReader.h"
#include "GameObject.h"
#include "Logger.h"
#include "Components.h"
#include "InputHandler.h"
#include "InputCommand.h"
#include "Commands.h"

void TestCollisionCallback(cp::GameObject* selfObject, cp::GameObject* otherObject, cp::CollisionBox::CollisionSide side)
{
	UNREFERENCED_PARAMETER(selfObject);
	UNREFERENCED_PARAMETER(otherObject);
	UNREFERENCED_PARAMETER(side);
	switch (side)
	{
	case cp::CollisionBox::all:
		break;
	case cp::CollisionBox::right:
		break;
	case cp::CollisionBox::up:
		break;
	case cp::CollisionBox::left:
		break;
	case cp::CollisionBox::down:
		break;
	}
}


void Game::LevelScene::LoadSceneData() const
{
	cp::Scene* scene = cp::SceneManager::GetInstance().CreateScene("LevelScene");
	BubbleBobbleLevelReader levelReader{};
	levelReader.ReadLevelData(scene, "LevelData/SeperatedLevelData.dat", "LevelData/LevelParallaxColors.png");

#pragma region FpsCounter

	cp::GameObject* fpsCounter = new cp::GameObject();
	scene->Add(fpsCounter);
	cp::Font* pFont = cp::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	cp::Text* pText = new cp::Text("0 FPS", pFont, SDL_Color{ 0, 0, 255 });
	pText->GetTexture2D()->AddLocalOffset(100, 100.f);
	fpsCounter->AddComponent(pText);
	fpsCounter->AddComponent(new cp::FrameRate());

#pragma endregion

#pragma region P1TestObj

	cp::GameObject* p1 = new cp::GameObject(cp::GameObjectType::Player1);
	scene->Add(p1);
	// color rect
	Uint8 r = 100, g = 100 , b = 100, a = 255;
	Uint32 p1Color = 0;
	p1Color = (p1Color | a) << 8;
	p1Color = (p1Color | b) << 8;
	p1Color = (p1Color | g) << 8;
	p1Color = (p1Color | r) << 0;
	p1->AddComponent(new cp::ColorRect2D(0, 0, 40, -40, p1Color));
	// collision
	p1->AddComponent(new cp::CollisionBox(32, -18, 3, 15, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_dynamic));
	p1->AddComponent(new cp::CollisionBox(7, -18, 3, 15, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_dynamic));
	p1->AddComponent(new cp::CollisionBox(7, -3, 25, 3, cp::CollisionBox::CollisionSide::down, cp::CollisionBox::CollisionType::_dynamic));
	p1->AddComponent(new cp::RigidBody(true));
	p1->SetCollisionCallback(std::bind(&TestCollisionCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	// translation
	p1->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->Translate(100, -50, 0);

	cp::Command* jmpCommand = new Game::JumpCommand();
	cp::Command* lftCommand = new Game::LeftCommand();
	cp::Command* rhtCommand = new Game::RightCommand();
	cp::Command* fireCommand = new Game::FireCommand();
	cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::KeyW, cp::InputState::Pressed, jmpCommand);
	cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::KeyA, cp::InputState::Hold, lftCommand);
	cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::KeyD, cp::InputState::Hold, rhtCommand);
	cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::LeftCtrl, cp::InputState::Pressed, fireCommand);

#pragma endregion

#pragma region P2TestObj

	cp::GameObject* p2 = new cp::GameObject(cp::GameObjectType::Player2);
	scene->Add(p2);
	// color rect
	r = 200, g = 200, b = 200, a = 255;
	Uint32 p2Color = 0;
	p2Color = (p2Color | a) << 8;
	p2Color = (p2Color | b) << 8;
	p2Color = (p2Color | g) << 8;
	p2Color = (p2Color | r) << 0;
	p2->AddComponent(new cp::ColorRect2D(0, 0, 40, -40, p2Color));
	// collision
	p2->AddComponent(new cp::CollisionBox(32, -18, 3, 15, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_dynamic));
	p2->AddComponent(new cp::CollisionBox(7, -18, 3, 15, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_dynamic));
	p2->AddComponent(new cp::CollisionBox(7, -3, 25, 3, cp::CollisionBox::CollisionSide::down, cp::CollisionBox::CollisionType::_dynamic));
	p2->AddComponent(new cp::RigidBody(true));
	p2->SetCollisionCallback(std::bind(&TestCollisionCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	// translation
	p2->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->Translate(100, -50, 0);

	cp::Command* controllerJmpCommand = new Game::ControllerJumpCommand();
	cp::Command* controllerHrzCommand = new Game::LeftCommand();
	cp::Command* controllerFireCommand = new Game::RightCommand();
	cp::InputHandler::GetInstance().AddInput(cp::ControllerButton::ButtonX, cp::InputState::Pressed, controllerJmpCommand);
	cp::InputHandler::GetInstance().AddInput(cp::ControllerAxis::LeftThumbStickX, controllerHrzCommand);
	cp::InputHandler::GetInstance().AddInput(cp::ControllerAxis::RightTrigger, controllerFireCommand);

#pragma endregion

#pragma region SideCollision

	cp::GameObject* sideCollision = new cp::GameObject(cp::GameObjectType::level);
	scene->Add(sideCollision);

	sideCollision->AddComponent(new cp::CollisionBox((20 * 2) - 3, 0, 3, 20 * 25, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_static));
	sideCollision->AddComponent(new cp::CollisionBox((20 * 30), 0, 3, 20 * 25, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_static));

#pragma endregion


}
