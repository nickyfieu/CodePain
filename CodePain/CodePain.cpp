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
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigDockingWithShift = true;

	ImGui_ImplSDL2_InitForMetal(m_Window); // done to fix a sertain assertion error

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
	auto lastTime = std::chrono::high_resolution_clock::now();

	bool doContinue = true;
	while (doContinue)
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		float elapsedSec = std::chrono::duration<float>(currentTime - lastTime).count();

		doContinue = input.ProcessInput();
		sceneManager.Update(elapsedSec);

#ifdef _DEBUG // all imgui stuf 
		
		ImGui::NewFrame();
		// debug dockspace
		ImGui::SetNextWindowPos(ImVec2(640.0f, 0.f));
		ImGui::SetNextWindowSizeConstraints(ImVec2(320.f, 500.f), ImVec2(320.f, 500.f));
		ImGui::Begin(" ", nullptr,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
			ImGuiID dockspace_id = ImGui::GetID(" ");
			ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), ImGuiDockNodeFlags_NoResize);
		ImGui::End();

// debug logger window
#ifdef DEBUGLOGGER
		logger.DrawLoggedInformation();
#endif
// debug level window
#ifdef DEBUGLEVELS
		ImGui::Begin("Level window", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		bool prevLevel = ImGui::Button("PreviousLevel");
		ImGui::SameLine();
		bool nextLevel = ImGui::Button("NextLevel");
		static int levelToDisplay = 1;
		int oldLevel = (int)levelToDisplay;
		if (prevLevel && levelToDisplay > 1) levelToDisplay--;
		if (nextLevel && levelToDisplay < 100) levelToDisplay++;
		ImGui::PushItemWidth(-100);
		ImGui::SliderInt("Current Level", &levelToDisplay, 1, 100);
		Scene* scene = sceneManager.GetActiveScene();
		if (scene)
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
						transform->SetPosition(0.f, float(heightLvl0 - (i * levelHeight)),0.f);
					}
				}
			}
		}
		ImGui::Checkbox("Visualize Collision", &renderer.gd_RenderCollisionBoxes);
		ImGui::End();
#endif

#endif
		renderer.Render();

		lastTime = currentTime;
	}

	Cleanup();
}
