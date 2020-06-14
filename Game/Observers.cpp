#include "CodePainPCH.h"
#include "Observer.h"
#include "GameObject.h"
#include "Observers.h"
#include "BubbleBobbleDataReader.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Logger.h"
#include "ResourceManager.h"
#include "States.h"

void Game::IdleEvent::OnNotify(const cp::GameObject* entity, cp::Event event)
{
	if (event != cp::Event::EVENT_IDLE)
		return;

	if (!(entity->GetType() == cp::GameObjectType::Player1 ||
		entity->GetType() == cp::GameObjectType::Player2))
		return;
}

void Game::SpawnLevelEnemies::OnNotify(const cp::GameObject*, cp::Event event)
{
	if (event != cp::Event::EVENT_SPAWN_ENEMIES)
		return;

	cp::Scene* currentScene = cp::SceneManager::GetInstance().GetActiveScene();
	BubbleBobbleDataReader& dataReader = BubbleBobbleDataReader::GetInstance();
	cp::GameManager& gameManager = cp::GameManager::GetInstance();

	currentScene->DeleteAllGameObjectsOfType(cp::GameObjectType::Npc);

	size_t currentLevel = gameManager.GetCurrentLevel();
	const std::vector<Game::BubbleBobbleEnemyData>& enemyLevelData = dataReader.GetLevelEnemyData(currentLevel - 1);

	size_t amountOfEnemies = enemyLevelData.size();
	for (size_t i = 0; i < amountOfEnemies; i++)
	{
		cp::GameObject* newEnemy = new cp::GameObject(cp::GameObjectType::Npc);

		Game::EnemyType type = enemyLevelData[i].enemyType;
		bool flyingType = (type == Game::EnemyType::Pulpul || type == Game::EnemyType::Monsta);

		// sprite component
		cp::SpriteComponent* sprite = new cp::SpriteComponent(cp::ResourceManager::GetInstance().LoadSDLTexture("CharacterData/EnemySpriteSheet.png"));
		cp::Texture2D* texture = sprite->GetTextureComponent();
		texture->AddLocalOffset(0.f, -40.f);
		texture->SetLocalScale(40.f, 40.f);
		SDL_Rect srcRect = texture->GetSrcRect();

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
		anim.yOffset = anim.spriteHeight * int(type);
		sprite->AddAnimation("WalkRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("WalkLeft", anim);
		anim.flipTexture = false;
		anim.xOffset = 16 * 4;
		sprite->AddAnimation("BubbleRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("BubbleLeft", anim);
		anim.xOffset = 16 * 8;
		sprite->AddAnimation("DieRight", anim);
		anim.flipTexture = true;
		sprite->AddAnimation("DieLeft", anim);
		if (enemyLevelData[i].isMovingLeft)
		{
			sprite->SetAnimation("WalkLeft");
		}
		else
		{
			sprite->SetAnimation("WalkRight");
		}
		sprite->UnPause();
		newEnemy->AddComponent(sprite);
		texture->SetOwner(newEnemy);

		cp::Transform* transform = newEnemy->GetComponent<cp::Transform>(cp::ComponentType::_Transform);
		transform->Translate(float(enemyLevelData[i].collumn * 20), -float(enemyLevelData[i].row * 20) + 20.f, 0.f);

		newEnemy->SetInactiveTimer(enemyLevelData[i].delay);

		// collision
		if (flyingType)
		{
			newEnemy->AddComponent(new cp::CollisionBox(9, -18, 22, 3, cp::CollisionBox::CollisionSide::up, cp::CollisionBox::CollisionType::_dynamic));
		}
		newEnemy->AddComponent(new cp::CollisionBox(31, -18, 3, 15, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_dynamic));
		newEnemy->AddComponent(new cp::CollisionBox(6, -18, 3, 15, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_dynamic));
		newEnemy->AddComponent(new cp::CollisionBox(6, -3, 28, 3, cp::CollisionBox::CollisionSide::down, cp::CollisionBox::CollisionType::_dynamic));
		newEnemy->AddComponent(new cp::CollisionBox(9, -15, 22, 12, cp::CollisionBox::CollisionSide::all, cp::CollisionBox::CollisionType::_dynamic));
		newEnemy->AddComponent(new cp::RigidBody(!flyingType));

		newEnemy->AddObserver(new Game::BottomLevelCollision());

		currentScene->Add(newEnemy);

		// adding base state
		switch (enemyLevelData[i].enemyType)
		{
		case Game::EnemyType::ZenChan:
		case Game::EnemyType::Hidegons:
			newEnemy->InitializeState(new Game::EnemyMovingState1());
			break;
		case Game::EnemyType::Banebou:
			break;
		case Game::EnemyType::Pulpul:
			break;
		case Game::EnemyType::Monsta:
			break;
		case Game::EnemyType::Drunk:
			break;
		case Game::EnemyType::Mighta:
			break;
		case Game::EnemyType::Invader:
			newEnemy->InitializeState(new Game::EnemyMovingState2());
			break;
		default:
			break;
		}


	}
}

void Game::BottomLevelCollision::OnNotify(const cp::GameObject* entity, cp::Event event)
{
	if (event != cp::Event::EVENT_COLLISION_OVERLAP)
		return;

	bool IsOverlappingWithLevel = false;
	std::vector<cp::CollisionBox*> colBoxex = entity->GetAllComponentsOfType<cp::CollisionBox>(cp::ComponentType::_CollisionBox);
	for (cp::CollisionBox* box : colBoxex)
	{
		cp::GameObject* overlap = box->GetOvelapObj();
		if (overlap != nullptr)
			if (overlap->GetType() == cp::GameObjectType::level)
				IsOverlappingWithLevel = true;
	}

	if (!IsOverlappingWithLevel)
		return;

	if (!entity->HasComponent<cp::RigidBody>(cp::ComponentType::_RigidBody))
		return;

	cp::Transform* transform = entity->GetComponent<cp::Transform>(cp::ComponentType::_Transform);
	glm::vec3 currPos = transform->GetPosition();
	transform->SetPosition(currPos.x, 20, currPos.z);
}
