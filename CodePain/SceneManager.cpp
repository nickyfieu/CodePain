#include "CodePainPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "InputHandler.h"

void cp::SceneManager::HandleInput(const cp::InputHandler& inputHandler)
{
	if (m_pActiveScene != nullptr)
		m_pActiveScene->HandleInput(inputHandler);
}

void cp::SceneManager::Update(const float elapsedSec)
{
	if (m_pActiveScene != nullptr) 
		m_pActiveScene->Update(elapsedSec);
}

void cp::SceneManager::FixedUpdate(const float elapsedSec)
{
	if (m_pActiveScene != nullptr)
		m_pActiveScene->FixedUpdate(elapsedSec);
}

void cp::SceneManager::Render() const
{
	if (m_pActiveScene != nullptr)
		m_pActiveScene->Render();
}

bool cp::SceneManager::SetActiveScene(const std::string& name)
{
	for (size_t i = 0; i < m_AmountOfScenes; i++)
	{
		if (m_Scenes[i]->GetName() == name)
		{
			m_pActiveScene = m_Scenes[i];
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
	m_AmountOfScenes++;
	(m_pActiveScene == nullptr) ? m_pActiveScene = pScene : nullptr;
	return pScene;
}

cp::SceneManager::~SceneManager()
{
	for (Scene* pToDelete : m_Scenes)
	{
		SAFE_DELETE(pToDelete);
	}
	if (!m_Scenes.empty())
		m_Scenes.clear();
}
