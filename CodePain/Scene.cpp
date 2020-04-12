#include "CodePainPCH.h"
#include "Scene.h"

void cp::Scene::ReserveGameObjects(size_t size)
{
	m_pObjects.reserve(size);
}

cp::Scene::Scene(const std::string& name) : m_Name(name) {}

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
}

void cp::Scene::Update(const float elapsedSec)
{
	for(GameObject* object : m_pObjects)
	{
		(object->GetIsActive()) ? object->Update(elapsedSec) : nullptr;
	}
}

void cp::Scene::Render() const
{
	for (const GameObject* object : m_pObjects)
	{
		(object->GetIsActive()) ? object->Render() : nullptr;
	}
}

cp::Scene::~Scene()
{
	for (GameObject* toDelete : m_pObjects)
	{
		SAFE_DELETE(toDelete);
	}
}

