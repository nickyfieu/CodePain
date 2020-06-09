#include "CodePainPCH.h"
#include "LevelScene.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "BubbleBobbleLevelReader.h"
#include "GameObject.h"
#include "Logger.h"
#include "CollisionBox.h"

void TestCollisionCallback(cp::GameObject* selfObject, cp::GameObject* otherObject, cp::CollisionBox::CollisionSide side)
{
	UNREFERENCED_PARAMETER(selfObject);
	UNREFERENCED_PARAMETER(otherObject);
	UNREFERENCED_PARAMETER(side);
	std::string debugMsg = "[Collision]Callback side: ";
	switch (side)
	{
	case cp::CollisionBox::all:
		debugMsg += "all";
		break;
	case cp::CollisionBox::right:
		debugMsg += "right";
		break;
	case cp::CollisionBox::up:
		debugMsg += "up";
		break;
	case cp::CollisionBox::left:
		debugMsg += "left";
		break;
	case cp::CollisionBox::down:
		debugMsg += "down";
		break;
	}
	cp::Logger::GetInstance().Log(cp::LogLevel::Debug, debugMsg);
}


void Game::LevelScene::LoadSceneData() const
{
	cp::Scene* scene = cp::SceneManager::GetInstance().CreateScene("LevelScene");
	BubbleBobbleLevelReader levelReader{};
	levelReader.ReadLevelData(scene, "LevelData/SeperatedLevelData.dat", "LevelData/LevelParallaxColors.png");

	// Fps Counter
	cp::GameObject* fpsCounter = new cp::GameObject();
	scene->Add(fpsCounter);
	cp::Font* pFont = cp::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	cp::Text* pText = new cp::Text("0 FPS", pFont, SDL_Color{ 0, 255, 0 });
	pText->GetTexture2D()->AddLocalOffset(40.f, 20.f);
	fpsCounter->AddComponent(pText);
	fpsCounter->AddComponent(new cp::FrameRate());

	// Collision Test Object
	cp::GameObject* collisionObject = new cp::GameObject();
	scene->Add(collisionObject);
	// color rect
	Uint8 r = 0, g = 0 , b = 0, a = 255;
	Uint32 color = 0;
	color = (color | a) << 8;
	color = (color | b) << 8;
	color = (color | g) << 8;
	color = (color | r) << 0;
	collisionObject->AddComponent(new cp::ColorRect2D(0, 0, 20, -20, color));
	// collision
	collisionObject->AddComponent(new cp::CollisionBox( 0,  -1, 20,  3, cp::CollisionBox::CollisionSide::down	, cp::CollisionBox::CollisionType::_dynamic));
	collisionObject->SetCollisionCallback(std::bind(&TestCollisionCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	// translation
	collisionObject->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->Translate(200, -50, 0);
}
