#pragma once
#include <string>

struct SDL_Window;
namespace cp
{
	class FrameRate;
	class CodePain final
	{
	public:
		CodePain() = default;
		~CodePain() = default;
		
		CodePain(const CodePain& other) = delete;
		CodePain(CodePain&& other) = delete;
		CodePain& operator=(const CodePain& other) = delete;
		CodePain& operator=(CodePain&& other) = delete;

		void Initialize();
		void Cleanup();
		void Run();

		inline bool IsInitialized() const { return m_IsInitialized; }
	private:
		//static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		bool m_IsInitialized = false;
		SDL_Window* m_Window{};

		// debug imgui stuf
		const size_t m_DebugLevelsHash = std::hash<std::string>{}("LevelScene");
		int m_MainLoopCPS = 0;
		int m_InputLoopCPS = 0;
		int m_DrawLoopCPS = 0;
		int m_FixedLoopCPS = 0;

		void ImGuiInit();
		void ImGuiDebug_Levels();
		void ImGuiUpdate();
		void ImGuiDebug_FrameRate();

	};
}