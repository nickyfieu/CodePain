#include "CodePainPCH.h"
#include "Scene.h"
#include "GameObject.h"

unsigned int cp::Scene::m_IdCounter = 0;

cp::Scene::Scene(const std::string& name) : m_Name(name) {}

void cp::Scene::Add(GameObject* object)
{
	m_pObjects.push_back(object);
}

void cp::Scene::Update(const float elapsedSec)
{
	for(GameObject* object : m_pObjects)
	{
		object->Update(elapsedSec);
	}
}

void cp::Scene::Render() const
{
	for (const GameObject* object : m_pObjects)
	{
		object->Render();
	}
}

cp::Scene::~Scene()
{
	for (GameObject* toDelete : m_pObjects)
	{
		SAFE_DELETE(toDelete);
	}
}

