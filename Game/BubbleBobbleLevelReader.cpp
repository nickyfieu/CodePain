#include "CodePainPCH.h"
#include "BubbleBobbleLevelReader.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Components.h"
#include "Logger.h"
#include <SDL_surface.h>

void Game::BubbleBobbleLevelReader::ReadLevelData(cp::Scene* pScene,const std::string& levelDataPath, const std::string& parallaxPath)
{
	const std::string LevelDataPath = cp::ResourceManager::GetInstance().GetDataPath() + levelDataPath;
	m_ReadWrite.ChangeFilePath(LevelDataPath);
	if (!m_ReadWrite.OpenFileToRead())
		return cp::Logger::GetInstance().Log(cp::LogLevel::Error, "BubbleBobbleLevelReader::ReadLevelData could not open file!");

	// assumption check ( amount of chars in a file should equal  bytes per level * amount of levels )
	int amountOfBytes = m_ReadWrite.AmountCharsInFile();
	if (amountOfBytes != int(m_BytesPerLevel * m_AmountOfLevels))
		return cp::Logger::GetInstance().Log(cp::LogLevel::Error, "BubbleBobbleLevelReader::ReadLevelData invalid level file!");

	// getting the parallax pixel data surface ( surface to be able to sample on a pixel position )
	SDL_Surface* pParallaxPixelData = cp::ResourceManager::GetInstance().LoadSDLSurface(parallaxPath);
	Uint32 colorRightParallax = 0;
	Uint32 colorLeftParallax = 0;

	// preemptive reserve of extra gameobjects
	pScene->ReserveGameObjects(int(m_AmountOfLevels * 1.5));

	unsigned char levelBits[m_AmountOfLevels]{};
	for (unsigned int i = 0; i < m_AmountOfLevels; i++)
	{
		cp::GameObject* levelGameObject = InitLevelGameObject(i,pScene);
		levelGameObject->SetActive(i == 0);
		ReadLevel(&levelBits[0]);

		ReadParallaxColors(i, colorRightParallax, colorLeftParallax, pParallaxPixelData);
		CalculateLevelCollisionAndParallaxBoxes(colorRightParallax, colorLeftParallax, levelGameObject, &levelBits[0]);

		CreateLevelTextures(levelGameObject, i, levelBits);
		CreateLevelCollision(levelGameObject);
	}
	m_ReadWrite.CloseFileToRead();
	if (pParallaxPixelData)
		SDL_FreeSurface(pParallaxPixelData);
}

void Game::BubbleBobbleLevelReader::ReadLevel(unsigned char levelBits[100])
{
	for (unsigned int j = 0; j < m_BytesPerLevel; j++)
	{
		unsigned char byte;
		m_ReadWrite.BinaryReading(byte);
		levelBits[j] = byte;
	}
}

void Game::BubbleBobbleLevelReader::ReadParallaxColors(unsigned int levelIndex, Uint32& color1, Uint32& color2, SDL_Surface* img)
{
	cp::ResourceManager& pResourceManager = cp::ResourceManager::GetInstance();
	color1 = pResourceManager.GetPixel(img, levelIndex, 0);
	color2 = pResourceManager.GetPixel(img, levelIndex, 1);
}

void Game::BubbleBobbleLevelReader::CalculateLevelCollisionAndParallaxBoxes(Uint32 colRight, Uint32 colDown, cp::GameObject* pLevelObj, const unsigned char levelBlocks[100])
{
	ReadCollisionSide(colRight, colDown, cp::CollisionBox::CollisionSide::left, pLevelObj, levelBlocks);
	ReadCollisionSide(colRight, colDown, cp::CollisionBox::CollisionSide::up, pLevelObj, levelBlocks);
	ReadCollisionSide(colRight, colDown, cp::CollisionBox::CollisionSide::right, pLevelObj, levelBlocks);
	ReadCollisionSide(colRight, colDown, cp::CollisionBox::CollisionSide::down, pLevelObj, levelBlocks);
}

void Game::BubbleBobbleLevelReader::ReadCollisionSide(Uint32 colRight, Uint32 colDown, cp::CollisionBox::CollisionSide side, cp::GameObject* pLevelObj, const unsigned char levelBlocks[100])
{
	std::vector<glm::tvec2<int>> collisionIndicies;
	const int colSize = 3;
	int col = 0, row = 0;
	int colMax = (int)m_LevelTilesWide, rowMax = (int)m_LevelTilesHigh;
	int indexCurrent = 0, indexToCheck = 0, cISize = 0;
	unsigned char currentMask = 0, toCheckMask = 0;
	InitReadColSideData(col, colMax, row, rowMax, side);
	bool upDown = side == cp::CollisionBox::CollisionSide::up || side == cp::CollisionBox::CollisionSide::down;
	int c = (upDown) ? row : col, r = (upDown) ? col : row;
	int cm = (upDown) ? rowMax : colMax, rm = (upDown) ? colMax : rowMax;

	for (c; c < cm; c++)
	{
		for (r; r < rm; r++)
		{
			if (upDown)
				UpdateReadColSideData(currentMask, toCheckMask, indexCurrent, indexToCheck, r, c, side);
			else
				UpdateReadColSideData(currentMask, toCheckMask, indexCurrent, indexToCheck, c, r, side);
			// current index = true && block to the right = false we have collision on the right side of this block
			if ((levelBlocks[indexCurrent] & currentMask) && !(levelBlocks[indexToCheck] & toCheckMask))
			{
				glm::tvec2<int> pos{ 0,0 };
				pos.x = (upDown) ? r * m_WindowTileSize : c * m_WindowTileSize;
				if (side == cp::CollisionBox::CollisionSide::right)
					pos.x += m_WindowTileSize - colSize;
				pos.y = (upDown) ? c * m_WindowTileSize : r * m_WindowTileSize;
				collisionIndicies.push_back(pos);
				cISize++;
			}
			else
			{
				UseColSideData(colRight, colDown, cISize, side, pLevelObj, collisionIndicies);
			}
		}
		UseColSideData(colRight, colDown, cISize, side, pLevelObj, collisionIndicies);
		r = (upDown) ? col : row;
	}
}

void Game::BubbleBobbleLevelReader::UseColSideData(Uint32 colRight, Uint32 colDown, int& colIndexSize, cp::CollisionBox::CollisionSide side, cp::GameObject* pLevelObj, std::vector<glm::tvec2<int>>& collisionIndicies)
{
	if (colIndexSize <= 0)
		return;
	// creates collision box + paralax effect
	const int colSize = 3;
	int x = collisionIndicies[0].x;
	int y = collisionIndicies[0].y;
	int width = colSize;
	int height = colSize;

	switch (side)
	{
	case cp::CollisionBox::right:
		height = (collisionIndicies[colIndexSize - 1].y + m_WindowTileSize) - y;
		CreateParallaxBoxTex(pLevelObj, side, x + colSize, y, m_ParallaxSize, height, colRight);
		break;
	case cp::CollisionBox::left:
		height = (collisionIndicies[colIndexSize - 1].y + m_WindowTileSize) - y;
		break;
	case cp::CollisionBox::down:
		y = collisionIndicies[0].y + m_WindowTileSize - colSize;
		width = (collisionIndicies[colIndexSize - 1].x + m_WindowTileSize) - x;
		CreateParallaxBoxTex(pLevelObj, side, x, y + colSize, width, m_ParallaxSize, colDown);
		break;
	case cp::CollisionBox::up:
		width = (collisionIndicies[colIndexSize - 1].x + m_WindowTileSize) - x;
		break;
	}
	m_pCollisionBoxes.push_back(new cp::CollisionBox(x, y, width, height, side, cp::CollisionBox::_static));
	collisionIndicies.clear();
	colIndexSize = 0;
}

void Game::BubbleBobbleLevelReader::InitReadColSideData(int& col, int& colMax, int& row, int& rowMax, cp::CollisionBox::CollisionSide side)
{
	switch (side)
	{
	case cp::CollisionBox::left:
		col++;
		break;
	case cp::CollisionBox::right:
		colMax--;
		break;
	case cp::CollisionBox::up:
		row++;
		break;
	case cp::CollisionBox::down:
		rowMax--;
		break;
	}
}

void Game::BubbleBobbleLevelReader::UpdateReadColSideData(unsigned char& currentMask, unsigned char& toCheckMask, int& indexCurrent, int& indexToCheck, int col, int row, cp::CollisionBox::CollisionSide side)
{
	currentMask = 0b10000000 >> (col % m_BitsInByte);
	toCheckMask = currentMask;
	indexCurrent = int(row * m_BytesWide + (col / m_BitsInByte));
	indexToCheck = indexCurrent;
	switch (side)
	{
	case cp::CollisionBox::right:
		toCheckMask = currentMask >> 1;
		if (currentMask & 0b00000001)
		{
			indexToCheck++;
			toCheckMask = 0b10000000;
		}
		break;
	case cp::CollisionBox::up:
		indexToCheck = indexCurrent - (int)m_BytesWide;
		break;
	case cp::CollisionBox::left:
		toCheckMask = currentMask << 1;
		if (currentMask & 0b10000000)
		{
			indexToCheck--;
			toCheckMask = 0b00000001;
		}
		break;
	case cp::CollisionBox::down:
		indexToCheck = indexCurrent + (int)m_BytesWide;
		break;
	}
}

void Game::BubbleBobbleLevelReader::CreateParallaxBoxTex(cp::GameObject* pLevelObj, cp::CollisionBox::CollisionSide side, int x, int y, int width, int height, Uint32 rgba)
{
	switch (side)
	{
	case cp::CollisionBox::right:
		for (unsigned int i = 0; i < m_AmountOfParallax; i++)
		{
			pLevelObj->AddComponent(new cp::ColorRect2D(x + (i * m_ParallaxSize), y + (i * m_ParallaxSize), width, height + m_ParallaxSize, rgba));
		}
		break;
	case cp::CollisionBox::down:
		for (unsigned int i = 0; i < m_AmountOfParallax; i++)
		{
			pLevelObj->AddComponent(new cp::ColorRect2D(x + (i * m_ParallaxSize), y + (i * m_ParallaxSize), width, height, rgba));
		}
		break;
	default:
		// we dont care about up / left for bubble bobble
		break;
	}
	
}

void Game::BubbleBobbleLevelReader::CreateLevelTextures(cp::GameObject* pLevelObj, unsigned int levelIndex, unsigned char levelBlocks[100])
{
	const unsigned int heightAndWithOfTile = 8;
	const unsigned int levelBytesWide = (m_LevelTilesWide / m_BitsInByte);
	SDL_Texture* blockTexture = cp::ResourceManager::GetInstance().LoadSDLTexture("LevelData/LevelBlocks.png");
	SDL_Texture* bigBlockTexture = cp::ResourceManager::GetInstance().LoadSDLTexture("LevelData/LevelBigBlocks.png");
	// source rect of each normal level block
	SDL_Rect srcNormalBlock{ (levelIndex % 10) * heightAndWithOfTile, int((levelIndex / 10) * heightAndWithOfTile), heightAndWithOfTile, heightAndWithOfTile};
	SDL_Rect srcBigBlock{ (levelIndex % 10) * heightAndWithOfTile * 2, int((levelIndex / 10) * heightAndWithOfTile * 2), heightAndWithOfTile * 2, heightAndWithOfTile * 2 };

	for (unsigned int i = 0; i < m_BytesPerLevel; i++)
	{
		// base destination rect for each byte in a level
		SDL_FRect dst{ float((i % m_BytesWide) * m_WindowTileSize * m_BitsInByte), float((i / m_BytesWide) * m_WindowTileSize), float(m_WindowTileSize), float(m_WindowTileSize) };
		unsigned char mask = 0b10000000;
		for (unsigned int k = 0; k < m_BitsInByte; k++)
		{
			if (levelBlocks[i] & mask)
			{	// true == big block / false == small block
				if ((((i % m_BytesWide) == 0) && ((levelBlocks[i] & 0b11000000) == 0b11000000) && (k == 0)) || (((i % m_BytesWide) == 3) && ((levelBlocks[i] & 0b00000011) == 0b00000011) && (k == 6)))
				{	
					if ((i % m_BytesWide) == 0)
						levelBlocks[i] = unsigned char(levelBlocks[i] & 0b00111111);
					else
						levelBlocks[i] = unsigned char(levelBlocks[i] & 0b11111100);
					
					float oldDSTW = dst.w;
					float oldDSTH = dst.h;
					int oldSRCBigBlock = srcBigBlock.h;

					dst.w *= 2; //assuming the block to the right is also true ( should be true always )
					if (i < (100 - 4)) // not out of range
					{
						if ((i % m_BytesWide) == 0)
							levelBlocks[i + 4] = unsigned char(levelBlocks[i + 4] & 0b00111111);
						else
							levelBlocks[i + 4] = unsigned char(levelBlocks[i + 4] & 0b11111100);

						dst.h *= 2; // we can assume if not out of range the block under the original one is true (should be true always )
					}
					else
					{
						srcBigBlock.h /= 2;
					}
				
					cp::Texture2D* block = new cp::Texture2D(bigBlockTexture);
					block->SetSrcRect(srcBigBlock);
					block->SetDstRect(dst);
					pLevelObj->AddComponent(block);

					dst.w = oldDSTW;
					dst.h = oldDSTH;
					srcBigBlock.h = oldSRCBigBlock;
				}
				else
				{
					cp::Texture2D* block = new cp::Texture2D(blockTexture);
					block->SetSrcRect(srcNormalBlock);
					block->SetDstRect(dst);
					pLevelObj->AddComponent(block);
				}
			}
			dst.x += m_WindowTileSize;
			mask = mask >> 1;
		}
	}
}

void Game::BubbleBobbleLevelReader::CreateLevelCollision(cp::GameObject* pLevelObj)
{
	for (cp::CollisionBox* pComponentToAdd : m_pCollisionBoxes)
	{
		pLevelObj->AddComponent(pComponentToAdd);
	}
	m_pCollisionBoxes.clear();
}

cp::GameObject* Game::BubbleBobbleLevelReader::InitLevelGameObject(unsigned int levelIndex, cp::Scene* pScene)
{
	// create a level game object and add it
	cp::GameObject* levelGameObject = new cp::GameObject(cp::GameObjectType::level);
	pScene->Add(levelGameObject);
	// adding a transform with height offset 
	// so that levels are stacked ( lvl 1 highest 2 below it etc )
	cp::Transform* levelTransform = levelGameObject->GetComponent<cp::Transform>(cp::ComponentType::_Transform);
	if (levelTransform != nullptr)
		levelTransform->SetPosition(0.f, -float(levelIndex * m_WindowTileSize * m_LevelTilesHigh), 0.f);
	return levelGameObject;
}