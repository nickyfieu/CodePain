#pragma once
namespace Game
{
	class LevelScene final
	{
	public:
		LevelScene() = default;
		~LevelScene() = default;

		LevelScene(const LevelScene& other) = delete;
		LevelScene(LevelScene&& other) = delete;
		LevelScene& operator=(const LevelScene& other) = delete;
		LevelScene& operator=(LevelScene&& other) = delete;

		void LoadSceneData() const;
	};
}