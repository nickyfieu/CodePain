#include "CodePainPCH.h"
#include "LevelScene.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "BubbleBobbleLevelReader.h"

void Game::LevelScene::LoadSceneData() const
{
	cp::Scene* scene = cp::SceneManager::GetInstance().CreateScene("LevelScene");
	BubbleBobbleLevelReader levelReader{};
	levelReader.ReadLevelData(scene);

	cp::GameObject* fpsCounter = new cp::GameObject();
	scene->Add(fpsCounter);
	cp::Font* pFont = cp::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	cp::Text* pText = new cp::Text("0 FPS", pFont, SDL_Color{ 0, 255, 0 });
	pText->SetLocalOffset(40.f, 20.f, 0.f);
	fpsCounter->AddComponent(pText);
	fpsCounter->AddComponent(new cp::FrameRate());

}
