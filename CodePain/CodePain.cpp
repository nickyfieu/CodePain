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

	float timer = 0.f;
	int mainCycleCounter = 0;
	int inputCycleCounter = 0;
	int drawCycleCounter = 0;
	int fixedCycleCounter = 0;

	float inputCycleTimer = 0.f;
	float drawCycleTimer = 0.f;
	float fixedCycleTimer = 0.f;
	const float inputCycleTime = 1.f / 1000.f;
	const float drawCycleTime = 1.f / 240.f;
	const float fixedCycleTime = 1.f / 40.f;

	bool doContinue = true;
	while (doContinue)
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		float elapsedSec = std::chrono::duration<float>(currentTime - lastTime).count();
		timer += elapsedSec;
		inputCycleTimer += elapsedSec;
		drawCycleTimer += elapsedSec;
		fixedCycleTimer += elapsedSec;

		if (timer > 1.0f)
		{
			m_MainLoopCPS = mainCycleCounter;
			mainCycleCounter = 0;
			m_InputLoopCPS = inputCycleCounter;
			inputCycleCounter = 0;
			m_DrawLoopCPS = drawCycleCounter;
			drawCycleCounter = 0;
			m_FixedLoopCPS = fixedCycleCounter;
			fixedCycleCounter = 0;
			timer = fmodf(timer - 1.f, 1.f);
		}

		bool inputCycleUpdate = (inputCycleTimer >= inputCycleTime);
		bool drawCycleUpdate = (drawCycleTimer >= drawCycleTime);
		bool fixedCycleUpdate = (fixedCycleTimer >= fixedCycleTime);
		
		mainCycleCounter++;

		if (inputCycleUpdate)
		{
			inputCycleCounter += int(inputCycleTimer / inputCycleTime);
			inputCycleTimer = fmodf(inputCycleTimer - inputCycleTime, inputCycleTime);
			doContinue = input.ProcessInput();
			sceneManager.HandleInput(inputHandler);
		}

		if (drawCycleUpdate)
		{
			int cycles = int(drawCycleTimer / drawCycleTime);
			drawCycleCounter += cycles;
			drawCycleTimer = fmodf(drawCycleTimer - drawCycleTime, drawCycleTime);
			sceneManager.Update(drawCycleTime * cycles);

#if defined(_DEBUG)
			ImGuiUpdate();
#endif
		}
		
		if (fixedCycleUpdate)
		{
			int cycles = int(fixedCycleTimer / fixedCycleTime);
			fixedCycleCounter += cycles;
			fixedCycleTimer = fmodf(fixedCycleTimer - fixedCycleTime, fixedCycleTime);
			sceneManager.FixedUpdate(fixedCycleTime);
		}

		if (drawCycleUpdate)
		{
			renderer.Render();
		}

		lastTime = currentTime;

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
	static int levelToDisplay = 1;
	int oldLevel = (int)levelToDisplay;

	ImGui::Begin("Level window", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	if (ImGui::Button("PreviousLevel") && levelToDisplay > 1)
		levelToDisplay--;
	ImGui::SameLine();
	if (ImGui::Button("NextLevel") && levelToDisplay < 100)
		levelToDisplay++;
	ImGui::PushItemWidth(-100);
	ImGui::SliderInt("Current Level", &levelToDisplay, 1, 100);

	if (scene && (oldLevel != levelToDisplay))
	{
		std::vector<GameObject*> level;
		level = scene->GetAllGameObjectsOfType(GameObjectType::level);
		size_t amountOfLevels = level.size();
		if (amountOfLevels > 0)
		{
			GameObject* gameObj = level.at(0);
			Transform* transform = gameObj->GetComponent<Transform>(ComponentType::_Transform);
			unsigned int levelHeight = 500;
			if (abs(transform->GetPosition().y / 500) != (levelToDisplay - 1))
			{
				// disabling old level and enableing new level
				level[oldLevel - 1]->SetActive(false);
				level[levelToDisplay - 1]->SetActive(true);

				float heightLvl0 = float((levelToDisplay - 1) * levelHeight);
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
