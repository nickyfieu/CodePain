#include "CodePainPCH.h"
#include "Scene1.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Components.h"
#include "BubbleBobbleLevelReader.h"

void Game::Scene1::LoadSceneData() const
{
	cp::Scene* scene = cp::SceneManager::GetInstance().CreateScene("Scene1");

	// background
	cp::GameObject* daeBackground = new cp::GameObject();
	SDL_Texture* pTex = cp::ResourceManager::GetInstance().LoadSDLTexture("background.jpg");
	daeBackground->AddComponent(new cp::Texture2D(pTex));
	scene->Add(daeBackground);

	// dae logo
	pTex = cp::ResourceManager::GetInstance().LoadSDLTexture("logo.png");
	cp::Texture2D* pTex2D = new cp::Texture2D(pTex);
	pTex2D->AddLocalOffset(216.f, -180.f);
	daeBackground->AddComponent(pTex2D);

	// Text
	cp::Font* pFont = cp::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	cp::Text* pText = new cp::Text("Programming 4 Assignment", pFont);
	pText->GetTexture2D()->AddLocalOffset(80.f, -20.f);
	daeBackground->AddComponent(pText);
	pText->Update(0.0f);
	pText->GetTexture2D()->SetDstRect(SDL_FRect{ 0, 0, 600, 300 });

	// Fps Counter
	cp::GameObject* fpsCounter = new cp::GameObject();
	scene->Add(fpsCounter);
	pFont = cp::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	pText = new cp::Text("0 FPS", pFont, SDL_Color{ 255, 255, 0 });
	pText->GetTexture2D()->AddLocalOffset(1.f, -1.f);
	fpsCounter->AddComponent(pText);
	fpsCounter->AddComponent(new cp::FrameRate());

}