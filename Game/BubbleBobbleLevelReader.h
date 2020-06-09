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

		// levelpath and parallaxpath both start from ../LevelData
		void ReadLevelData(cp::Scene* pScene, const std::string& levelDataPath, const std::string& parallaxPath);

		void ReadEnemyData() const;

	private:
		static const unsigned int m_AmountOfLevels = 100;
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

		void ReadLevel(unsigned char* levelBits);
		cp::GameObject* InitLevelGameObject(unsigned int levelIndex, cp::Scene* pScene);

		void CalculateLevelCollisionAndParallaxBoxes(Uint32 colRight, Uint32 colDown, cp::GameObject* pLevelObj,const unsigned char levelBlocks[100]);
		void CreateParallaxBoxTex(cp::GameObject* pLevelObj, cp::CollisionBox::CollisionSide side, int x, int y, int width, int height, Uint32 rgba);
		void CreateLevelTextures(cp::GameObject* pLevelObj, unsigned int levelIndex, unsigned char levelBlocks[100]);
		void CreateLevelCollision(cp::GameObject* pLevelObj);

		void ReadCollisionSide(Uint32 colRight, Uint32 colDown, cp::CollisionBox::CollisionSide side, cp::GameObject* pLevelObj, const unsigned char levelBlocks[100]);
		void UseColSideData(Uint32 colRight, Uint32 colDown, int& colIndexSize, cp::CollisionBox::CollisionSide side, cp::GameObject* pLevelObj, std::vector<glm::tvec2<int>>& collisionIndicies);
		void InitReadColSideData(int& col, int& colMax, int& row, int& rowMax, cp::CollisionBox::CollisionSide side);
		void UpdateReadColSideData(unsigned char& currentMask, unsigned char& toCheckMask, int& indexCurrent, int& indexToCheck, int col, int row, cp::CollisionBox::CollisionSide side);

		void ReadParallaxColors(unsigned int levelIndex, Uint32& color1, Uint32& color2, SDL_Surface* img);

	};
}