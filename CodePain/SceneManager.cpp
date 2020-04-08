#include "CodePainPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void cp::SceneManager::Update(const float elapsedSec)
{
	for(Scene* scene : m_Scenes)
	{
		scene->Update(elapsedSec);
	}
}

void cp::SceneManager::Render()
{
	for (const Scene* scene : m_Scenes)
	{
		scene->Render();
	}
}

cp::Scene* cp::SceneManager::CreateScene(const std::string& name)
{
	const size_t posNewScene = m_Scenes.size();
	cp::Scene* pScene = new Scene(name);
	m_Scenes.push_back(pScene);
	return pScene;
}

cp::SceneManager::~SceneManager()
{
	for (Scene* pToDelete : m_Scenes)
	{
		SAFE_DELETE(pToDelete);
	}
	if (!m_Scenes.empty()) m_Scenes.clear();
}
