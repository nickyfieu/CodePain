#include "CodePainPCH.h"
#include "GameManager.h"
#include "GameObject.h"

cp::GameManager::~GameManager()
{
	SAFE_DELETE(m_ManagerObj);
}

void cp::GameManager::Initialize()
{
	m_ManagerObj = new GameObject();
	m_CurrentLevel = 1;
}

void cp::GameManager::SetCurrentLevel(size_t currentLevel)
{
	m_CurrentLevel = currentLevel;
}
