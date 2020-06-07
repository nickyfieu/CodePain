#include "CodePainPCH.h"
#include "SceneLoader.h"
#include "SceneManager.h"
#include "Scenes.h"

#define LoadScene1
#define LoadLevelScene

void Game::SceneLoader::LoadAllDefinedScenes() const
{
#ifdef LoadScene1
	Scene1 scene1{};
	scene1.LoadSceneData();
	cp::SceneManager::GetInstance().SetActiveScene("Scene1");
#endif

#ifdef LoadLevelScene
	LevelScene levelTestScene{};
	levelTestScene.LoadSceneData();
	cp::SceneManager::GetInstance().SetActiveScene("LevelScene");
#endif


}
