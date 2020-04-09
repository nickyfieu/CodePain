#include "CodePainPCH.h"
#include "SceneLoader.h"
#include "Scenes.h"

#define LoadScene1

void Game::SceneLoader::LoadAllDefinedScenes() const
{
#ifdef LoadScene1
	Scene1 scene{};
	scene.LoadSceneData();
#endif
}
