#include "CodePainPCH.h"
#include "CodePain.h"
#include <chrono>
#include "InputManager.h"
#include "InputHandler.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_thread.h>
#include "GameObject.h"
#include "Components.h"
#include "Scene.h"
#include "Logger.h"
#include "Observer.h"
#include "GameManager.h"
#ifdef _DEBUG
	#include "Imgui\imgui.h"
	#include "Imgui_Sdl\imgui_sdl.h"
	#include "Imgui_Sdl\imgui_impl_sdl.h"
#endif

#define DEBUGLOGGER
#define DEBUGLEVELS


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
	ImGuiInit();
	AllocConsole();
#endif

	ResourceManager::GetInstance().Init("../Data/");

	m_IsInitialized = true;
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
	Logger& logger = Logger::GetInstance();
	if (m_IsInitialized == false)
	{
		logger.Log(LogLevel::Error, "Levels will not be loaded see warning for more information!", true);
		logger.Log(LogLevel::Warning, "CodePain::Run Initialize was not called in the main.cpp!", true);
		Initialize();
	}
	
	Renderer& renderer = Renderer::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();
	InputManager& input = InputManager::GetInstance();
	InputHandler& inputHandler = InputHandler::GetInstance();
	
	input.Initialize();

	auto lastTime = std::chrono::high_resolution_clock::now();


	float T1 = 0.f, T2 = 0.f, T3 = 0.f, T4 = 0.f;
	int C1 = 0, C2 = 0, C3 = 0, C4 = 0;
	const float mainCycleTime = 1.f;
	const float inputCycleTime = 1.f / 1200.f;
	const float drawCycleTime = 1.f / 240.f;
	const float fixedCycleTime = 1.f / 40.f;

	bool doContinue = true;
	while (doContinue)
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		float elapsedSec = std::chrono::duration<float>(currentTime - lastTime).count();
		T1 += elapsedSec;
		T2 += elapsedSec;
		T3 += elapsedSec;
		T4 += elapsedSec;
		C1++;

		if(T2 >= inputCycleTime)
		{
			C2++;
			T2 -= inputCycleTime;
			doContinue = input.ProcessInput();
			sceneManager.HandleInput(inputHandler);
		}

		if (T3 >= drawCycleTime)
		{
			C3++;
			T3 = 0.f;
			sceneManager.Update(drawCycleTime);

#if defined(_DEBUG)
			ImGuiUpdate();
#endif
		
			if (T4 >= fixedCycleTime)
			{
				C4++;
				T4 = 0.f;
				sceneManager.FixedUpdate(fixedCycleTime);
			}

			renderer.Render();
		}

		lastTime = currentTime;

		if (T1 >= mainCycleTime)
		{
			T1 = 0.f; T2 = 0.f; T3 = 0.f; T4 = 0.f;
			m_MainLoopCPS = C1; m_InputLoopCPS = C2; m_DrawLoopCPS = C3; m_FixedLoopCPS = C4;
			C1 = 0; C2 = 0; C3 = 0; C4 = 0;
		}

	}

	inputHandler.Cleanup();
	Cleanup();
}

void cp::CodePain::ImGuiInit()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigDockingWithShift = true;
#if defined(_DEBUG)
	ImGui_ImplSDL2_InitForMetal(m_Window); // done to fix a sertain assertion error
#endif
}

void cp::CodePain::ImGuiUpdate()
{
	ImGui::NewFrame();
	// debug dockspace
	ImGui::SetNextWindowPos(ImVec2(640.0f, 0.f));
	ImGui::SetNextWindowSizeConstraints(ImVec2(320.f, 500.f), ImVec2(320.f, 500.f));
	ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	ImGuiID dockspace_id = ImGui::GetID(" ");
	ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), ImGuiDockNodeFlags_NoResize);
	ImGui::End();

#if defined(DEBUGLOGGER)
	Logger& logger = Logger::GetInstance();
	logger.DrawLoggedInformation();
#endif

	SceneManager& sceneManager = SceneManager::GetInstance();
	Scene* scene = sceneManager.GetActiveScene();

	if (scene->GetNameHash() == m_DebugLevelsHash)
		ImGuiDebug_Levels();

	ImGuiDebug_FrameRate();
}

void cp::CodePain::ImGuiDebug_FrameRate()
{
	ImGui::Begin("Cycles Per Second(CPS)");
	ImGui::Text("Main loop  	: %i cps", m_MainLoopCPS);
	ImGui::Text("Input  		: %i cps", m_InputLoopCPS);
	ImGui::Text("Sound  		: %i cps", 0);
	ImGui::Text("Draw/Update	: %i cps", m_DrawLoopCPS);
	ImGui::Text("FixedUpdate	: %i cps", m_FixedLoopCPS);
	ImGui::End();
}

void cp::CodePain::ImGuiDebug_Levels()
{
	Renderer& renderer = Renderer::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();
	Scene* scene = sceneManager.GetActiveScene();
	GameManager& gameManager = GameManager::GetInstance();
	int currentLevel = gameManager.GetCurrentLevel();
	int oldLevel = currentLevel;

	ImGui::Begin("Level window", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	if (ImGui::Button("PreviousLevel") && currentLevel > 1)
	{
		currentLevel--;
		scene->DeleteAllGameObjectsOfType(cp::GameObjectType::Npc);
	}
	ImGui::SameLine();
	if (ImGui::Button("NextLevel") && currentLevel < 100)
	{
		currentLevel++;
		scene->DeleteAllGameObjectsOfType(cp::GameObjectType::Npc);
	}
	ImGui::SameLine();
	bool spawnEnemies = ImGui::Button("SpawnEnemies");
	ImGui::PushItemWidth(-100);
	ImGui::SliderInt("Current Level", &currentLevel, 1, 100);

	gameManager.SetCurrentLevel(currentLevel);
	if (spawnEnemies)
	{
		gameManager.GetManagerObj()->NotifyObservers(cp::Event::EVENT_SPAWN_ENEMIES);
	}

	if (scene && (oldLevel != currentLevel))
	{
		std::vector<GameObject*> level;
		level = scene->GetAllGameObjectsOfType(GameObjectType::level);
		size_t amountOfLevels = level.size();
		if (amountOfLevels > 0)
		{
			GameObject* gameObj = level.at(0);
			Transform* transform = gameObj->GetComponent<Transform>(ComponentType::_Transform);
			unsigned int levelHeight = 500;
			if (abs(transform->GetPosition().y / 500) != (currentLevel - 1))
			{
				// disabling old level and enableing new level
				level[oldLevel - 1]->SetActive(false);
				level[currentLevel - 1]->SetActive(true);

				float heightLvl0 = float((currentLevel - 1) * levelHeight);
				for (size_t i = 0; i < amountOfLevels; i++)
				{
					gameObj = level.at(i);
					transform = gameObj->GetComponent<Transform>(ComponentType::_Transform);
					transform->SetPosition(0.f, float(heightLvl0 - (i * levelHeight)), 0.f);
				}
			}
		}
	}
	ImGui::Checkbox("Visualize Collision", &renderer.gd_RenderCollisionBoxes);
	ImGui::Checkbox("Visualize Texture Tiles", &renderer.gd_RenderTextureTiles);
	ImGui::End();
}
