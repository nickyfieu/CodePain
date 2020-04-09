#pragma once
struct SDL_Window;
namespace cp
{
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
		SDL_Window* m_Window{};
		bool m_IsInitialized = false;
	};
}