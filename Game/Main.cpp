#include "CodePain.h"
#include "SceneLoader.h"
#include "GameManager.h"

#if _DEBUG
#include <vld.h>
#endif

int main(int, char* [])
{
	Game::SceneLoader sceneLoader{};
	cp::CodePain engine;
	cp::GameManager& gameManager = cp::GameManager::GetInstance();

	// InitEngine
	engine.Initialize();
	gameManager.Initialize();

	if (engine.IsInitialized())
	{
		// InitGame
		sceneLoader.LoadAllDefinedScenes();

		//	update
		engine.Run();
	}

	return 0;
}