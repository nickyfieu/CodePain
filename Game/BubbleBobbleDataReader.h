#pragma once
#include "BinaryReaderWriter.h"
#include "BubbleBobbleData.h"
#include "Singleton.h"
#include "Scene.h"

namespace Game
{
	class BubbleBobbleDataReader final : public cp::Singleton<BubbleBobbleDataReader>
	{
	public:
		virtual ~BubbleBobbleDataReader() = default;

		BubbleBobbleDataReader(const BubbleBobbleDataReader& other) = delete;
		BubbleBobbleDataReader(BubbleBobbleDataReader&& other) = delete;
		BubbleBobbleDataReader& operator=(const BubbleBobbleDataReader& other) = delete;
		BubbleBobbleDataReader& operator=(BubbleBobbleDataReader&& other) = delete;

		// levelpath and parallaxpath both start from ../LevelData
		void ReadLevelData(cp::Scene* pScene, const std::string& levelDataPath, const std::string& parallaxPath);

		void ReadEnemyData(const std::string& enemyDataPath);

		const std::vector<BubbleBobbleEnemyData>& GetLevelEnemyData(size_t level) const;

	private:
		friend class cp::Singleton<BubbleBobbleDataReader>;
		BubbleBobbleDataReader() = default;

		std::unordered_map<size_t, std::vector<BubbleBobbleEnemyData>> m_EnemyDataContainer;

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

		void ReadLevel(unsigned char levelBits[100]);
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