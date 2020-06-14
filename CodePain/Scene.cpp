#include "CodePainPCH.h"
#include "Scene.h"
#include "InputHandler.h"
#include "GameManager.h"
#include "Observer.h"
#include <vector>
#include <string>

#ifdef MULTI_THREADING
	#include <future>
#endif

void cp::Scene::ReserveGameObjects(size_t size)
{
	m_pObjects.reserve(size);
}

cp::Scene::Scene(const std::string& name) 
	: m_Name(name)
	, m_NameHash(std::hash<std::string>{}(name))
{
}

std::vector<cp::GameObject*> cp::Scene::GetAllGameObjectsOfType(GameObjectType type)
{
	size_t size = m_pObjects.size();
	std::vector<GameObject*> vec;
	vec.reserve(size / 2);
	for (size_t i = 0; i < size; i++)
	{
		(m_pObjects[i]->GetType() == type) ? vec.push_back(m_pObjects[i]) : nullptr;
	}
	return vec;
}

void cp::Scene::DeleteAllGameObjectsOfType(GameObjectType type)
{
	size_t amountToRemove = 0;
	std::vector<GameObject*> toRemove;
	for (size_t i = 0; i < m_AmountOfObjects; i++)
	{
		if (m_pObjects[i]->GetType() == type)
		{
			amountToRemove++;
			toRemove.push_back(m_pObjects[i]);
		}
	}

	for (size_t i = 0; i < amountToRemove; i++)
	{
		DeleteGameObject(toRemove[i]);
	}
}

void cp::Scene::DeleteGameObject(GameObject* ref)
{
	m_AmountOfObjects--;
	m_pObjects.erase(std::find(m_pObjects.begin(), m_pObjects.end(), ref));
	SAFE_DELETE(ref);

	GameManager& manager = GameManager::GetInstance();
	if (manager.GetManagerObj())
		manager.GetManagerObj()->NotifyObservers(cp::Event::EVENT_OBJ_DESTROYED);
}

void cp::Scene::SwitchLevel(size_t levelIndex)
{
	std::vector<GameObject*> levels = GetAllGameObjectsOfType(GameObjectType::level);
	GameManager& manager = GameManager::GetInstance();
	GameObject* currLevel = nullptr;
	for (GameObject* level : levels)
	{
		if (level->GetIsActive())
			currLevel = level;
	}
	GameObject* nextLevel = levels[levelIndex-1];
	if (nextLevel != nullptr)
	{
		if (currLevel)
			currLevel->SetActive(false);
		nextLevel->SetActive(true);
		manager.SetCurrentLevel(levelIndex);
	}
}

void cp::Scene::Add(GameObject* object)
{
	m_pObjects.push_back(object);
	m_AmountOfObjects++;
}

#ifdef MULTI_THREADING
static void UpdateActiveObject(cp::GameObject* activeObject, float elapsedSec)
{
	activeObject->UpdateState();
	activeObject->Update(elapsedSec);
}

static void FixedUpdateActiveObject(cp::GameObject* activeObject, float elapsedSec)
{
	activeObject->FixedUpdate(elapsedSec);
}
#endif

void cp::Scene::HandleInput(const cp::InputHandler& inputHandler)
{
	for (size_t i = 0; i < m_AmountOfObjects; i++)
		if (m_pObjects[i]->GetIsActive())
			inputHandler.HandleInput(m_pObjects[i]);
}

void cp::Scene::Update(const float elapsedSec)
{
#ifdef MULTI_THREADING
	std::vector<std::future<void>> updateFutures;
#endif
	size_t amountToRemove = 0;
	std::vector<GameObject*>removeReferences;

	for (size_t i = 0; i < m_AmountOfObjects; i++)
	{
		if (m_pObjects[i]->GetToDestroy())
		{
			amountToRemove++;
			removeReferences.push_back(m_pObjects[i]);
		}
	}

	for (size_t i = 0; i < amountToRemove; i++)
	{
		DeleteGameObject(removeReferences[i]);
	}

	for(size_t i = 0; i < m_AmountOfObjects; i++)
	{
		if (m_pObjects[i]->GetIsActive())
		{
#ifdef MULTI_THREADING
			updateFutures.push_back(std::async(std::launch::async, UpdateActiveObject, m_pObjects[i], elapsedSec));
#else
			m_pObjects[i]->UpdateState();
			m_pObjects[i]->Update(elapsedSec);
#endif
		}
	}
}

void cp::Scene::FixedUpdate(float elapsedSec)
{
#ifdef MULTI_THREADING
	std::vector<std::future<void>> updateFutures;
#endif

	for (size_t i = 0; i < m_AmountOfObjects; i++)
	{
		if (m_pObjects[i]->GetIsActive())
		{
#ifdef MULTI_THREADING
			updateFutures.push_back(std::async(std::launch::async, FixedUpdateActiveObject, m_pObjects[i], elapsedSec));
#else
			m_pObjects[i]->FixedUpdate(elapsedSec);
#endif
		}
	}
}

void cp::Scene::Render() const
{
	for (size_t i = 0; i < m_AmountOfObjects; i++)
		if (m_pObjects[i]->GetIsActive())
			m_pObjects[i]->Render();
}

cp::Scene::~Scene()
{
	for (GameObject* toDelete : m_pObjects)
	{
		SAFE_DELETE(toDelete);
	}
}