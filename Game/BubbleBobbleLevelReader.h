#pragma once
#include "BinaryReaderWriter.h"
#include "Scene.h"

namespace Game
{
	class BubbleBobbleLevelReader final
	{
	public:
		BubbleBobbleLevelReader() = default;
		~BubbleBobbleLevelReader() = default;

		BubbleBobbleLevelReader(const BubbleBobbleLevelReader& other) = delete;
		BubbleBobbleLevelReader(BubbleBobbleLevelReader&& other) = delete;
		BubbleBobbleLevelReader& operator=(const BubbleBobbleLevelReader& other) = delete;
		BubbleBobbleLevelReader& operator=(BubbleBobbleLevelReader&& other) = delete;

		void ReadLevelData(cp::Scene* pScene);

		void ReadEnemyData() const;

	private:
		const unsigned int m_LevelTilesWide = 32;
		const unsigned int m_LevelTilesHigh = 25;

		const unsigned int m_WindowSizeX = 640;
		const unsigned int m_WindowTileSize = m_WindowSizeX / m_LevelTilesWide;

		BinaryReaderWriter m_ReadWrite{};

		void CalculateLevelCollision(cp::GameObject* pLevelObj,const bool levelBlocks[800]);
	};
}