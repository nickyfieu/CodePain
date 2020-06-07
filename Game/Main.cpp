#include "CodePain.h"
#include "SceneLoader.h"

#if _DEBUG
#include <vld.h>
#endif

int main(int, char* [])
{
	Game::SceneLoader sceneLoader{};
	cp::CodePain engine;

	// InitEngine
	engine.Initialize();

	if (engine.IsInitialized())
	{
		// InitGame
		sceneLoader.LoadAllDefinedScenes();

		//	update
		engine.Run();
	}

	return 0;
}