#include "CodePainPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void cp::SceneManager::Update(const float elapsedSec)
{
	(m_pActiveScene == nullptr) ? nullptr : m_pActiveScene->Update(elapsedSec);
}

void cp::SceneManager::Render() const
{
	(m_pActiveScene == nullptr) ? nullptr : m_pActiveScene->Render();
}

bool cp::SceneManager::SetActiveScene(const std::string& name)
{
	for (Scene* pToCheck : m_Scenes)
	{
		if (pToCheck->GetName() == name)
		{
			m_pActiveScene = pToCheck;
			return true;
		}
	}
	return false;
}

cp::Scene* cp::SceneManager::CreateScene(const std::string& name)
{
	const size_t posNewScene = m_Scenes.size();
	cp::Scene* pScene = new Scene(name);
	m_Scenes.push_back(pScene);
	(m_pActiveScene == nullptr) ? m_pActiveScene = pScene : nullptr;
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
