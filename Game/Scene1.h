#pragma once

namespace Game
{
	class Scene1 final
	{
	public:
		Scene1() = default;
		~Scene1() = default;

		Scene1(const Scene1& other) = delete;
		Scene1(Scene1&& other) = delete;
		Scene1& operator=(const Scene1& other) = delete;
		Scene1& operator=(Scene1&& other) = delete;

		void LoadSceneData() const;
	};
}