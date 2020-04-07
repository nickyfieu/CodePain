#include "CodePainPCH.h"
#include "CodePain.h"
#include <chrono>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Components.h"
#include "Scene.h"

void cp::CodePain::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void cp::CodePain::LoadGame() const
{
	cp::Scene* scene = SceneManager::GetInstance().CreateScene("Game");

	// background
	GameObject* daeBackground = new GameObject();
	scene->Add(daeBackground);

	SDL_Texture* pTex = ResourceManager::GetInstance().LoadSDLTexture("background.jpg");
	daeBackground->AddComponent(new Texture2D(pTex));

	// dae logo
	pTex = ResourceManager::GetInstance().LoadSDLTexture("logo.png");
	Texture2D* pTex2D = new Texture2D(pTex);
	daeBackground->AddComponent(pTex2D);
	pTex2D->SetLocalOffset(216.f, 180.f, 0.f);

	cp::Font* pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	Text* pText = new Text("Programming 4 Assignment", pFont);
	pText->SetLocalOffset(80.f, 20.f, 0.f);
	daeBackground->AddComponent(pText);

	GameObject* fpsCounter = new GameObject();
	scene->Add(fpsCounter);
	pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	pText = new Text("0 FPS", pFont, SDL_Color{255, 255, 0});
	pText->SetLocalOffset(1.f, 1.f, 0.f);
	fpsCounter->AddComponent(pText);
	fpsCounter->AddComponent(new FrameRate());
}

void cp::CodePain::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void cp::CodePain::Run()
{
#if _DEBUG
	AllocConsole();
#endif

	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		Renderer& renderer = Renderer::GetInstance();
		SceneManager& sceneManager = SceneManager::GetInstance();
		InputManager& input = InputManager::GetInstance();
		auto lastTime = std::chrono::high_resolution_clock::now();

		bool doContinue = true;
		while (doContinue)
		{
			const auto currentTime = std::chrono::high_resolution_clock::now();
			float elapsedSec = std::chrono::duration<float>(currentTime - lastTime).count();

			doContinue = input.ProcessInput();
			sceneManager.Update(elapsedSec);
			renderer.Render();
			
			lastTime = currentTime;
		}
	}

	Cleanup();
}
