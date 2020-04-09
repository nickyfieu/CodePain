#pragma once

namespace Game
{
	class SceneLoader final
	{
	public:
		SceneLoader() = default;
		~SceneLoader() = default;

		SceneLoader(const SceneLoader& other) = delete;
		SceneLoader(SceneLoader&& other) = delete;
		SceneLoader& operator=(const SceneLoader& other) = delete;
		SceneLoader& operator=(SceneLoader&& other) = delete;

		void LoadAllDefinedScenes() const;
	};
}