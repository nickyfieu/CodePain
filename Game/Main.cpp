#include "CodePain.h"
#include "SceneLoader.h"

#if _DEBUG
#include <vld.h>
#endif

int main(int, char* [])
{
	Game::SceneLoader sceneLoader{};
	cp::CodePain engine;
	engine.Initialize();
	engine.IsInitialized() ? sceneLoader.LoadAllDefinedScenes() : nullptr;
	engine.Run();
	return 0;
}