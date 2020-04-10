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

// InitGame
	engine.IsInitialized() ? sceneLoader.LoadAllDefinedScenes() : nullptr;

//	update
	engine.Run();

	return 0;
}