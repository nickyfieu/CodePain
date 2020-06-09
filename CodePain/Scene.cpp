#include "CodePainPCH.h"
#include "Scene.h"
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

void cp::Scene::Add(GameObject* object)
{
	m_pObjects.push_back(object);
	m_AmountOfObjects++;
}

#ifdef MULTI_THREADING
static void UpdateActiveObject(cp::GameObject* activeObject, float elapsedSec)
{
	activeObject->Update(elapsedSec);
}

static void FixedUpdateActiveObject(cp::GameObject* activeObject, float elapsedSec)
{
	activeObject->FixedUpdate(elapsedSec);
}
#endif

void cp::Scene::Update(const float elapsedSec)
{
#ifdef MULTI_THREADING
	std::vector<std::future<void>> updateFutures;
#endif

	for(size_t i = 0; i < m_AmountOfObjects; i++)
	{
		if (m_pObjects[i]->GetIsActive())
		{
#ifdef MULTI_THREADING
			updateFutures.push_back(std::async(std::launch::async, UpdateActiveObject, m_pObjects[i], elapsedSec));
#else
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