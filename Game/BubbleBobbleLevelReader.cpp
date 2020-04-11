#include "CodePainPCH.h"
#include "BubbleBobbleLevelReader.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Components.h"
#include <SDL_surface.h>

void Game::BubbleBobbleLevelReader::ReadLevelData(cp::Scene* pScene)
{
	cp::ResourceManager& pResourceManager = cp::ResourceManager::GetInstance();
	const std::string LevelDataPath = cp::ResourceManager::GetInstance().GetDataPath() + "LevelData/SeperatedLevelData.dat";

	const unsigned int amountOfLevels = 100;

	m_ReadWrite.ChangeFilePath(LevelDataPath);
	m_ReadWrite.OpenFileToRead();

	bool levelBits[800]{0};

	SDL_Surface* pParallaxPixelData = pResourceManager.LoadSDLSurface("LevelData/LevelParallaxColors.png");

	for (unsigned int i = 0; i < amountOfLevels; i++)
	{
		Uint32 colRight = pResourceManager.GetPixel(pParallaxPixelData, i , 0);
		Uint32 colLeft = pResourceManager.GetPixel(pParallaxPixelData, i, 1);

		cp::GameObject* levelGameObject = new cp::GameObject(cp::GameObjectType::level);
		pScene->Add(levelGameObject);

		(i == 0) ? levelGameObject->SetActive(true) : levelGameObject->SetActive(false);

		cp::Transform* levelTransform = levelGameObject->GetComponent<cp::Transform>(cp::ComponentType::_Transform);
		// add a height offset 
		(levelTransform != nullptr) ? levelTransform->SetPosition(0.f, -float(i * m_WindowTileSize * m_LevelTilesHigh), 0.f) : nullptr;

		for (unsigned int j = 0; j < m_BytesPerLevel; j++)
		{
			unsigned char byte;
			m_ReadWrite.BinaryReading(byte);
			unsigned char mask = 0b10000000;
			for (unsigned int k = 0; k < m_BitsInByte; k++)
			{
				(byte & mask) ? levelBits[(j * 8) + k] = true: levelBits[(j * 8) + k] = false;
				mask = mask >> 1;
			}
		}
		CalculateLevelCollisionAndParallaxBoxes(levelGameObject, levelBits, colRight, colLeft);
		CreateLevelTextures(levelGameObject, i, levelBits);
		// vector was used to save collision due to the way of how rendering works 
		// if not done like this you cannot debug render the collision ( level would render ontop )
		CreateLevelCollision(levelGameObject);
	}
	m_ReadWrite.CloseFileToRead();

	if (pParallaxPixelData) SDL_FreeSurface(pParallaxPixelData);
}

void Game::BubbleBobbleLevelReader::CalculateLevelCollisionAndParallaxBoxes(cp::GameObject* pLevelObj, const bool levelBlocks[800], Uint32 colRight, Uint32 colDown)
{
	std::vector<glm::tvec2<int>> collisionIndicies;
	int cISize{0};

	m_pCollisionBoxes.reserve((m_LevelTilesWide * m_LevelTilesHigh) / 10);

	// read right collision
	// if block to right of current is false ( valid right collision ) push back collision vector with a pos
	// read the next one BELOW it ( level data[0] = top left )
	// when the last one in the row is read create a collision box component  (if size > 0)
	// OR
	// when the next block in the row is not a valid create a collision box component
	// when creating a collision box component clear the vector after that
	// creating the collision box component requires this data for a right collision box
	// left = collisionIndicies[first].x + tile size - some value
	// bottom = collisionIndicies[first].y 
	// width = some value
	// height = (collisionIndicies[last].y + blockHeight) - bottom

	cp::CollisionSide side = cp::CollisionSide::right;
	const int colSize = 3;
	for (int col = 0; col < (int)m_LevelTilesWide - 1; col++) // -1 due to no posible right collision on the full right side
	{
		for (int row = 0; row < (int)m_LevelTilesHigh; row++)
		{
			int indexCurrent{row * (int)m_LevelTilesWide + col};
			int indexToCheck{ indexCurrent + 1 };

			if (levelBlocks[indexCurrent] && !levelBlocks[indexToCheck]) // current index = true && block to the right = false we have collision on the right side of this block
			{
				glm::tvec2<int> pos{ 0,0 };
				pos.x = col * m_WindowTileSize + m_WindowTileSize - colSize;
				pos.y = row * m_WindowTileSize;
				collisionIndicies.push_back(pos);
				cISize++;
			}
			else // check if collision indicies size is greater than 0 if true make collision box
			{
				if (cISize > 0)
				{
					int x = collisionIndicies[0].x;
					int y = collisionIndicies[0].y;
					int width = colSize;
					int height = (collisionIndicies[cISize - 1].y + m_WindowTileSize) - y;
					m_pCollisionBoxes.push_back(new cp::CollisionBox(x, y, width, height, side));
					CreateParallaxBoxTex(pLevelObj, side, x + colSize, y , m_ParallaxSize, height, colRight);
					collisionIndicies.clear();
					cISize = 0;
				}
			}
		}
		
		if (cISize > 0)
		{
			int x = collisionIndicies[0].x;
			int y = collisionIndicies[0].y;
			int width = colSize;
			int height = (collisionIndicies[cISize - 1].y + m_WindowTileSize) - y;
			m_pCollisionBoxes.push_back(new cp::CollisionBox(x, y, width, height, side));
			CreateParallaxBoxTex(pLevelObj, side, x + colSize, y, m_ParallaxSize, height, colRight);
			collisionIndicies.clear();
			cISize = 0;
		}
	}

	// read up collision 
	// if block ontop of current is false ( valid up collision ) push back collision vector with a pos
	// read the next one RIGHT it ( level data[0] = top left )
	// when the last one in the collumn is read create a collision box component (if size > 0) 
	// OR
	// when the next block in the collumn is not a valid create a collision box component
	// when creating a collision box component clear the vector after that
	// creating the collision box component requires this data for a up collision box
	// left = collisionIndicies[first].x
	// bottom = collisionIndicies[first].y 
	// width = (collisionIndicies[last].x + blockWidth) - left
	// height = some value
	side = cp::CollisionSide::up;
	for (int row = 1; row < (int)m_LevelTilesHigh; row++) // starts at 1 as you cannot check row 0 ( -1 doesnt exist )
	{
		for (int col = 0; col < (int)m_LevelTilesWide; col++)
		{
			int indexCurrent{ row * (int)m_LevelTilesWide + col };
			int indexToCheck{ indexCurrent - (int)m_LevelTilesWide }; // checks the one up

			if (levelBlocks[indexCurrent] && !levelBlocks[indexToCheck]) // current index = true && block ontop = false we have collision on the up side of this block
			{
				glm::tvec2<int> pos{ 0,0 };
				pos.x = col * m_WindowTileSize;
				pos.y = row * m_WindowTileSize;
				collisionIndicies.push_back(pos);
				cISize++;
			}
			else // check if collision indicies size is greater than 0 if true make collision box
			{
				if (cISize > 0)
				{
					int x = collisionIndicies[0].x;
					int y = collisionIndicies[0].y;
					int width = (collisionIndicies[cISize - 1].x + m_WindowTileSize) - x;
					int height = colSize;
					m_pCollisionBoxes.push_back(new cp::CollisionBox(x, y, width, height, side));
					collisionIndicies.clear();
					cISize = 0;
				}
			}
		}

		if (cISize > 0)
		{
			int x = collisionIndicies[0].x;
			int y = collisionIndicies[0].y;
			int width = (collisionIndicies[cISize - 1].x + m_WindowTileSize) - x;
			int height = colSize;
			m_pCollisionBoxes.push_back(new cp::CollisionBox(x, y, width, height, side));
			collisionIndicies.clear();
			cISize = 0;
		}
	}

	// read left collision
	// if block to left of current is false ( valid left collision ) push back collision vector with a pos
	// read the next one BELOW it ( level data[0] = top left )
	// when the last one in the row is read create a collision box component  (if size > 0)
	// OR
	// when the next block in the row is not a valid create a collision box component
	// when creating a collision box component clear the vector after that
	// creating the collision box component requires this data for a right collision box
	// left = collisionIndicies[first].x 
	// bottom = collisionIndicies[first].y 
	// width = some value
	// height = (collisionIndicies[last].y + blockHeight) - bottom

	side = cp::CollisionSide::left;
	for (int col = 1; col < (int)m_LevelTilesWide; col++) // starts at 1 due to col 0 doesnt have a -1
	{
		for (int row = 0; row < (int)m_LevelTilesHigh; row++)
		{
			int indexCurrent{ row * (int)m_LevelTilesWide + col };
			int indexToCheck{ indexCurrent -1 };

			if (levelBlocks[indexCurrent] && !levelBlocks[indexToCheck]) // current index = true && block to the left = false we have collision on the left side of this block
			{
				glm::tvec2<int> pos{ 0,0 };
				pos.x = col * m_WindowTileSize;
				pos.y = row * m_WindowTileSize;
				collisionIndicies.push_back(pos);
				cISize++;
			}
			else // check if collision indicies size is greater than 0 if true make collision box
			{
				if (cISize > 0)
				{
					int x = collisionIndicies[0].x;
					int y = collisionIndicies[0].y;
					int width = colSize;
					int height = (collisionIndicies[cISize - 1].y + m_WindowTileSize) - y;
					m_pCollisionBoxes.push_back(new cp::CollisionBox(x, y, width, height, side));
					collisionIndicies.clear();
					cISize = 0;
				}
			}
		}

		if (cISize > 0)
		{
			int x = collisionIndicies[0].x;
			int y = collisionIndicies[0].y;
			int width = colSize;
			int height = (collisionIndicies[cISize - 1].y + m_WindowTileSize) - y;
			m_pCollisionBoxes.push_back(new cp::CollisionBox(x, y, width, height, side));
			collisionIndicies.clear();
			cISize = 0;
		}
	}

	// read down collision
	// if block below the current is false ( valid down collision ) push back collision vector with a pos
	// read the next one RIGHT of it ( level data[0] = top left )
	// when the last one in the column is read create a collision box component  (if size > 0)
	// OR
	// when the next block in the column is not a valid create a collision box component
	// when creating a collision box component clear the vector after that
	// creating the collision box component requires this data for a down collision box
	// left = collisionIndicies[first].x 
	// bottom = collisionIndicies[first].y + blockSide - some value
	// width = (collisionIndicies[last].y + blockSize) - bottom
	// height = some value

	side = cp::CollisionSide::down;

	for (int row = 0; row < (int)m_LevelTilesHigh - 1; row++) // -1 due to there beeing no block below the last tile high
	{
		for (int col = 0; col < (int)m_LevelTilesWide; col++)
		{
			int indexCurrent{ row * (int)m_LevelTilesWide + col };
			int indexToCheck{ indexCurrent + (int)m_LevelTilesWide }; // checks the one up

			if (levelBlocks[indexCurrent] && !levelBlocks[indexToCheck]) // current index = true && block ontop = false we have collision on the up side of this block
			{
				glm::tvec2<int> pos{ 0,0 };
				pos.x = col * m_WindowTileSize;
				pos.y = row * m_WindowTileSize;
				collisionIndicies.push_back(pos);
				cISize++;
			}
			else // check if collision indicies size is greater than 0 if true make collision box
			{
				if (cISize > 0)
				{
					int x = collisionIndicies[0].x;
					int y = collisionIndicies[0].y + m_WindowTileSize - colSize;
					int width = (collisionIndicies[cISize - 1].x + m_WindowTileSize) - x;
					int height = colSize;
					m_pCollisionBoxes.push_back(new cp::CollisionBox(x, y, width, height, side));
					CreateParallaxBoxTex(pLevelObj, side, x, y + colSize, width, m_ParallaxSize, colDown);
					collisionIndicies.clear();
					cISize = 0;
				}
			}
		}

		if (cISize > 0)
		{
			int x = collisionIndicies[0].x;
			int y = collisionIndicies[0].y + m_WindowTileSize - colSize;
			int width = (collisionIndicies[cISize - 1].x + m_WindowTileSize) - x;
			int height = colSize;
			m_pCollisionBoxes.push_back(new cp::CollisionBox(x, y, width, height, side));
			CreateParallaxBoxTex(pLevelObj, side, x, y + colSize, width, m_ParallaxSize, colDown);
			collisionIndicies.clear();
			cISize = 0;
		}
	}

}

void Game::BubbleBobbleLevelReader::CreateParallaxBoxTex(cp::GameObject* pLevelObj, cp::CollisionSide side, int x, int y, int width, int height, Uint32 rgba)
{
	switch (side)
	{
	case cp::right:
		for (unsigned int i = 0; i < m_AmountOfParallax; i++)
		{
			pLevelObj->AddComponent(new cp::ColRect2D(x + (i * m_ParallaxSize), y + (i * m_ParallaxSize), width, height + m_ParallaxSize, rgba));
		}
		break;
	case cp::down:
		for (unsigned int i = 0; i < m_AmountOfParallax; i++)
		{
			pLevelObj->AddComponent(new cp::ColRect2D(x + (i * m_ParallaxSize), y + (i * m_ParallaxSize), width, height, rgba));
		}
		break;
	default:
		// we dont care about up / left for bubble bobble
		break;
	}
	
}

void Game::BubbleBobbleLevelReader::CreateLevelTextures(cp::GameObject* pLevelObj, unsigned int levelIndex, bool levelBlocks[800])
{
	const unsigned int heightAndWithOfTile = 8;

	SDL_Texture* blockTexture = cp::ResourceManager::GetInstance().LoadSDLTexture("LevelData/LevelBlocks.png");

	// source rect of each normal level block
	SDL_Rect srcNormalBlock{};
	srcNormalBlock.x = (levelIndex % 10) * heightAndWithOfTile;
	srcNormalBlock.y = (levelIndex / 10) * heightAndWithOfTile;
	srcNormalBlock.w = heightAndWithOfTile;
	srcNormalBlock.h = heightAndWithOfTile;

	SDL_Texture* bigBlockTexture = cp::ResourceManager::GetInstance().LoadSDLTexture("LevelData/LevelBigBlocks.png");

	SDL_Rect srcBigBlock{};
	srcBigBlock.x = (levelIndex % 10) * heightAndWithOfTile * 2;
	srcBigBlock.y = (levelIndex / 10) * heightAndWithOfTile * 2;
	srcBigBlock.w = heightAndWithOfTile * 2;
	srcBigBlock.h = heightAndWithOfTile * 2;

	const unsigned int levelBytesWide = (m_LevelTilesWide / m_BitsInByte);

	for (unsigned int i = 0; i < m_BytesPerLevel; i++)
	{
		// base destination rect for each byte in a level
		SDL_FRect dst{};
		dst.x = float((i % (m_LevelTilesWide / m_BitsInByte)) * m_WindowTileSize * m_BitsInByte);
		dst.y = float((i / (m_LevelTilesWide / m_BitsInByte)) * m_WindowTileSize);
		dst.w = float(m_WindowTileSize);
		dst.h = float(m_WindowTileSize);
		for (unsigned int k = 0; k < m_BitsInByte; k++)
		{
			unsigned int blockID = (i * m_BitsInByte) + k;
			if (levelBlocks[blockID])
			{
				// if true big block
				// if false small bock
				if (((blockID % m_LevelTilesWide) == 0) || ((blockID % m_LevelTilesWide) == (m_LevelTilesWide - 2)))
				{
					levelBlocks[blockID] = false;
					levelBlocks[blockID + 1] = false;
					
					float oldDSTW = dst.w;
					float oldDSTH = dst.h;
					int oldSRCBigBlock = srcBigBlock.h;

					dst.w *= 2; //assuming the block to the right is also true ( should be true always )
					blockID += m_LevelTilesWide;
					if (blockID < 800) // not out of range
					{
						levelBlocks[blockID] = false;
						levelBlocks[blockID + 1] = false;
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
