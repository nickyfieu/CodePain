#include "CodePainPCH.h"
#include "LevelScene.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "BubbleBobbleLevelReader.h"
#include "GameObject.h"
#include "Logger.h"
#include "Components.h"

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

	// Collision Test Object
	cp::GameObject* collisionObject = new cp::GameObject();
	scene->Add(collisionObject);
	// color rect
	Uint8 r = 0, g = 0 , b = 255, a = 255;
	Uint32 color = 0;
	color = (color | a) << 8;
	color = (color | b) << 8;
	color = (color | g) << 8;
	color = (color | r) << 0;
	collisionObject->AddComponent(new cp::ColorRect2D(0, 0, 20, -20, color));
	// collision
	collisionObject->AddComponent(new cp::CollisionBox(0, -21, 20, 3, cp::CollisionBox::CollisionSide::up, cp::CollisionBox::CollisionType::_dynamic));
	collisionObject->AddComponent(new cp::CollisionBox(18, -20, 3, 20, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_dynamic));
	collisionObject->AddComponent(new cp::CollisionBox(-1, -20, 3, 20, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_dynamic));
	collisionObject->AddComponent(new cp::CollisionBox(0, -1, 20, 3, cp::CollisionBox::CollisionSide::down, cp::CollisionBox::CollisionType::_dynamic));
	collisionObject->AddComponent(new cp::RigidBody(true));
	collisionObject->SetCollisionCallback(std::bind(&TestCollisionCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	// translation
	collisionObject->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->Translate(200, -50, 0);
}
