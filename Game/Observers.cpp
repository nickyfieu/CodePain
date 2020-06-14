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
#include "SceneManager.h"

void Game::SpawnLevelEnemies::OnNotify(const cp::GameObject*, cp::Event event)
{
	if (event != cp::Event::EVENT_SPAWN_ENEMIES)
		return;

	cp::Scene* currentScene = cp::SceneManager::GetInstance().GetActiveScene();
	BubbleBobbleDataReader& dataReader = BubbleBobbleDataReader::GetInstance();
	cp::GameManager& gameManager = cp::GameManager::GetInstance();

	// level ui stuf 
	std::vector<cp::GameObject*> uiObjectVector = currentScene->GetAllGameObjectsOfType(cp::GameObjectType::UIElements);
	const size_t bubbleBobbleStatisticsIndex = 2 - 1;
	std::vector<cp::Text*> textComponents = uiObjectVector[bubbleBobbleStatisticsIndex]->GetAllComponentsOfType<cp::Text>(cp::ComponentType::_Text);
	cp::Text* levelText = textComponents[2];
	if (levelText)
	{
		levelText->SetText(std::to_string(gameManager.GetCurrentLevel()));
	}

	// enemies stuf

	currentScene->DeleteAllGameObjectsOfType(cp::GameObjectType::Npc);

	size_t currentLevel = gameManager.GetCurrentLevel();
	const std::vector<Game::BubbleBobbleEnemyData>& enemyLevelData = dataReader.GetLevelEnemyData(currentLevel - 1);

	cp::GameObject* player1Obj = currentScene->GetAllGameObjectsOfType(cp::GameObjectType::Player1)[0];
	cp::GameObject* player2Obj = currentScene->GetAllGameObjectsOfType(cp::GameObjectType::Player2)[0];

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

		newEnemy->AddObserver(new Game::TopBottomLevelCollision());
		newEnemy->AddObserver(new Game::TopBottomLevelCollision());

		currentScene->Add(newEnemy);

		// adding base state
		switch (enemyLevelData[i].enemyType)
		{
		case Game::EnemyType::Hidegons:
			newEnemy->InitializeState(new Game::EnemyMovingState1());
			newEnemy->AddComponent(new cp::Score(300));
			break;
		case Game::EnemyType::Drunk:
			newEnemy->InitializeState(new Game::EnemyMovingState1());
			newEnemy->AddComponent(new cp::Score(700));
			break;
		case Game::EnemyType::Mighta:
			newEnemy->InitializeState(new Game::EnemyMovingState1());
			newEnemy->AddComponent(new cp::Score(200));
			break;
		case Game::EnemyType::ZenChan:
			newEnemy->InitializeState(new Game::EnemyMovingState1());
			newEnemy->AddComponent(new cp::Score(100));
			break;
		case Game::EnemyType::Banebou:
			newEnemy->InitializeState(new Game::EnemyMovingState3());
			newEnemy->AddComponent(new cp::Score(150));
			break;
		case Game::EnemyType::Pulpul:
			// bool bit 1 => up = false / down = true
			newEnemy->InitializeState(new Game::EnemyMovingState4(enemyLevelData[i].isMovingDown, 200.f,100.f));
			newEnemy->AddComponent(new cp::Score(400));
			break;
		case Game::EnemyType::Monsta:
			newEnemy->InitializeState(new Game::EnemyMovingState4(enemyLevelData[i].isMovingDown, 125.f, 125.f));
			newEnemy->AddComponent(new cp::Score(500));
			break;
		case Game::EnemyType::Invader:
			newEnemy->InitializeState(new Game::EnemyMovingState2());
			newEnemy->AddComponent(new cp::Score(600));
			break;
		default:
			break;
		}

		// target component
		cp::Target* pTarget = new cp::Target();
		if (player1Obj)
			pTarget->AddTarget(player1Obj);
		if (player2Obj)
			pTarget->AddTarget(player2Obj);
		newEnemy->AddComponent(pTarget);
	}
}

void Game::TopBottomLevelCollision::OnNotify(const cp::GameObject* entity, cp::Event event)
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

	cp::RigidBody* rigid = entity->GetComponent<cp::RigidBody>(cp::ComponentType::_RigidBody);
	if (rigid == nullptr)
		return;

	cp::Transform* transform = entity->GetComponent<cp::Transform>(cp::ComponentType::_Transform);
	glm::vec3 currPos = transform->GetPosition();
	if (currPos.y >= 250.f)
	{
		if (rigid->GetForce().y < 0)
		{
			transform->SetPosition(currPos.x, 5, currPos.z);
		}
	}
	else
	{
		if (rigid->GetForce().y > 0)
		{
			transform->SetPosition(currPos.x, -525, currPos.z);
		}
	}
}

void Game::PlayerHealthChange::OnNotify(const cp::GameObject* entity, cp::Event event)
{
	if (event != cp::Event::EVENT_HP_CHANGE)
		return;

	if ((entity->GetType() != cp::GameObjectType::Player1) && (entity->GetType() != cp::GameObjectType::Player2))
		return;

	cp::Health* health = entity->GetComponent<cp::Health>(cp::ComponentType::_Health);
	if (health == nullptr)
		return;

	cp::Scene* currentScene = cp::SceneManager::GetInstance().GetActiveScene();
	std::vector<cp::GameObject*> uiObjectVector = currentScene->GetAllGameObjectsOfType(cp::GameObjectType::UIElements);
	const size_t bubbleBobbleStatisticsIndex = 2 - 1;
	std::vector<cp::Text*> textComponents = uiObjectVector[bubbleBobbleStatisticsIndex]->GetAllComponentsOfType<cp::Text>(cp::ComponentType::_Text);
	if (entity->GetType() == cp::GameObjectType::Player1)
	{
		textComponents[0]->SetText(std::to_string(health->GetHealth()) + " HP");
	}
	else if (entity->GetType() == cp::GameObjectType::Player2)
	{
		textComponents[1]->SetText(std::to_string(health->GetHealth()) + " HP");
	}
}

void Game::PlayerOverlap::OnNotify(const cp::GameObject* entity, cp::Event event)
{
	if (event != cp::Event::EVENT_COLLISION_OVERLAP)
		return;

	if ((entity->GetType() != cp::GameObjectType::Player1) && (entity->GetType() != cp::GameObjectType::Player2))
		return;

	cp::Health* playerHealth = entity->GetComponent<cp::Health>(cp::ComponentType::_Health);
	if (playerHealth == nullptr)
		return;

	auto collisionBoxes = entity->GetAllComponentsOfType<cp::CollisionBox>(cp::ComponentType::_CollisionBox);
	cp::CollisionBox* overlapBox = nullptr;
	for (auto box : collisionBoxes)
	{
		if (box->GetCollisionSide() == cp::CollisionBox::CollisionSide::all && box->GetCollisionType() == cp::CollisionBox::CollisionType::_dynamic)
			overlapBox = box;
	}

	if (overlapBox == nullptr)
		return;
	
	cp::GameObject* overlapObj = overlapBox->GetOvelapObj();
	if (overlapObj == nullptr)
		return;

	if (overlapObj->GetType() == cp::GameObjectType::Npc)
	{
		playerHealth->AddHealth(-1);
		if (entity->GetType() == cp::GameObjectType::Player1)
		{
			//entity->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->SetPosition(60, -480, 0);
		}
		else if (entity->GetType() == cp::GameObjectType::Player2)
		{
			//entity->GetComponent<cp::Transform>(cp::ComponentType::_Transform)->SetPosition(540, -480, 0);
		}

		// testing purposes

		cp::Score* enemyScore = overlapObj->GetComponent<cp::Score>(cp::ComponentType::_Score);
		cp::Score* playerScore = entity->GetComponent<cp::Score>(cp::ComponentType::_Score);
		playerScore->AddScore(enemyScore->GetScore());
		overlapObj->SetDestroy(true);

	}

}

void Game::ProceedToNextLevel::OnNotify(const cp::GameObject*, cp::Event event)
{
	if (event != cp::Event::EVENT_OBJ_DESTROYED)
		return;

	cp::Scene* currentScene = cp::SceneManager::GetInstance().GetActiveScene();
	size_t amountOfEnemies = currentScene->GetAllGameObjectsOfType(cp::GameObjectType::Npc).size();
	
	if (amountOfEnemies == 0)
	{
		size_t nextLevel = cp::GameManager::GetInstance().GetCurrentLevel() + 1;
		if (nextLevel == 99)
		{
			currentScene->SwitchLevel(1);
			//cp::GameManager::GetInstance().SetGameState(cp::GameManager::GameState::Menu);
			//currentScene->GetAllGameObjectsOfType(cp::GameObjectType::UIElements)[0]->SetActive(true);
		}
		else
			currentScene->SwitchLevel(nextLevel);
	}
}

void Game::PlayerScoreChange::OnNotify(const cp::GameObject* entity, cp::Event event)
{
	if (event != cp::Event::EVENT_SCORE_CHANGE)
		return;

	if ((entity->GetType() != cp::GameObjectType::Player1) && (entity->GetType() != cp::GameObjectType::Player2))
		return;

	cp::Score* score = entity->GetComponent<cp::Score>(cp::ComponentType::_Score);
	if (score == nullptr)
		return;

	cp::Scene* currentScene = cp::SceneManager::GetInstance().GetActiveScene();
	std::vector<cp::GameObject*> uiObjectVector = currentScene->GetAllGameObjectsOfType(cp::GameObjectType::UIElements);
	const size_t bubbleBobbleStatisticsIndex = 2 - 1;
	std::vector<cp::Text*> textComponents = uiObjectVector[bubbleBobbleStatisticsIndex]->GetAllComponentsOfType<cp::Text>(cp::ComponentType::_Text);
	if (entity->GetType() == cp::GameObjectType::Player1)
	{
		textComponents[3]->SetText(std::to_string(score->GetScore()));
	}
	else if (entity->GetType() == cp::GameObjectType::Player2)
	{
		textComponents[4]->SetText(std::to_string(score->GetScore()));
	}
}
