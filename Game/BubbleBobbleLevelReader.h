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
		const unsigned int m_BitsInByte = 8;
		const unsigned int m_LevelTilesWide = 32;
		const unsigned int m_LevelTilesHigh = 25;
		const unsigned int m_BytesPerLevel = (m_LevelTilesWide * m_LevelTilesHigh) / m_BitsInByte;
		const unsigned int m_BytesWide = (m_LevelTilesWide / m_BitsInByte);

		const unsigned int m_WindowSizeX = 640;
		const unsigned int m_WindowTileSize = m_WindowSizeX / m_LevelTilesWide;
		const unsigned int m_AmountOfParallax = 5;
		const unsigned int m_ParallaxSize = 2;

		BinaryReaderWriter m_ReadWrite{};

		std::vector<cp::CollisionBox*> m_pCollisionBoxes;

		void CalculateLevelCollisionAndParallaxBoxes(cp::GameObject* pLevelObj,const unsigned char levelBlocks[100], Uint32 colRight, Uint32 colDown);
		void CreateParallaxBoxTex(cp::GameObject* pLevelObj, cp::CollisionSide side, int x, int y, int width, int height, Uint32 rgba);
		void CreateLevelTextures(cp::GameObject* pLevelObj, unsigned int levelIndex, unsigned char levelBlocks[100]);
		void CreateLevelCollision(cp::GameObject* pLevelObj);
	};
}