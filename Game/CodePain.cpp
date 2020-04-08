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
#include "Logger.h"
#ifdef _DEBUG
	#include "Imgui\imgui.h"
	#include "Imgui_Sdl\imgui_sdl.h"
	#include "Imgui_Sdl\imgui_impl_sdl.h"
#endif

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
#ifdef _DEBUG
		960,
		500,
#else
		640,
		500,
#endif
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);

#ifdef _DEBUG
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigDockingWithShift = true;

	ImGui_ImplSDL2_InitForMetal(m_Window); // done to fix a sertain assertion error
#endif
}

/**
 * Code constructing the scene world starts here
 */
void cp::CodePain::LoadGame() const
{
	cp::Scene* scene = SceneManager::GetInstance().CreateScene("FPSScene");
	// background
	GameObject* daeBackground = new GameObject();
	SDL_Texture* pTex = ResourceManager::GetInstance().LoadSDLTexture("background.jpg");
	daeBackground->AddComponent(new Texture2D(pTex));
	scene->Add(daeBackground);
	// dae logo
	pTex = ResourceManager::GetInstance().LoadSDLTexture("logo.png");
	Texture2D* pTex2D = new Texture2D(pTex);
	pTex2D->SetLocalOffset(216.f, 180.f, 0.f);
	daeBackground->AddComponent(pTex2D);
	// Text
	cp::Font* pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	Text* pText = new Text("Programming 4 Assignment", pFont);
	pText->SetLocalOffset(80.f, 20.f, 0.f);
	daeBackground->AddComponent(pText);
	pText->Update(0.0f);
	pText->GetTexture2D()->SetDstRect(SDL_Rect{ 0, 0, 600, 300 });
	// Fps Counter
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
		Logger& logger = Logger::GetInstance();
		auto lastTime = std::chrono::high_resolution_clock::now();

		LogLevel logLevel;
		char msg[40];
		logLevel = LogLevel::Debug;
		for (int i = 0; i < 1000; i++)
		{
			sprintf_s(msg, "%i", i);
			logger.Log(logLevel, (std::string)msg, true);
		}
		logLevel = LogLevel::Info;
		for (int i = 0; i < 10; i++)
		{
			sprintf_s(msg, "%i", i);
			logger.Log(logLevel, (std::string)msg, true);
		}
		logLevel = LogLevel::Warning;
		for (int i = 0; i < 10; i++)
		{
			sprintf_s(msg, "%i", i);
			logger.Log(logLevel, (std::string)msg, true);
		}
		logLevel = LogLevel::Critical;
		for (int i = 0; i < 10; i++)
		{
			sprintf_s(msg, "%i", i);
			logger.Log(logLevel, (std::string)msg, true);
		}

		bool doContinue = true;
		while (doContinue)
		{
			const auto currentTime = std::chrono::high_resolution_clock::now();
			float elapsedSec = std::chrono::duration<float>(currentTime - lastTime).count();

			doContinue = input.ProcessInput();
			sceneManager.Update(elapsedSec);
#ifdef _DEBUG
			ImGui::NewFrame();
			ImGui::SetNextWindowPos(ImVec2(640.0f, 0.f));
			ImGui::SetNextWindowSizeConstraints(ImVec2(320.f, 500.f), ImVec2(320.f, 500.f));
			ImGui::Begin("My DockSpace", nullptr,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
			ImGuiID dockspace_id = ImGui::GetID("DebugDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), ImGuiDockNodeFlags_NoResize);
			ImGui::End();
			// debug window
			logger.DrawLoggedInformation();
#endif

			renderer.Render();

			lastTime = currentTime;
		}
	}

	Cleanup();
}
