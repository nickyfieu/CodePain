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
#endif

#ifdef LoadLevelScene
	LevelScene levelTestScene{};
	levelTestScene.LoadSceneData();
#endif


	cp::SceneManager::GetInstance().SetActiveScene("LevelScene");
}
