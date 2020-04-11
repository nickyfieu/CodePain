#include "CodePainPCH.h"
#include "BubbleBobbleLevelReader.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Components.h"

void Game::BubbleBobbleLevelReader::ReadLevelData(cp::Scene* pScene)
{
	const std::string LevelDataPath = cp::ResourceManager::GetInstance().GetDataPath() + "LevelData/SeperatedLevelData.dat";

	const unsigned int amountOfLevels = 100;
	const unsigned int bitsInByte = 8;
	const unsigned int bytesPerLevel = (m_LevelTilesWide * m_LevelTilesHigh) / bitsInByte;
	const unsigned int heightAndWithOfTile = 8;

	m_ReadWrite.ChangeFilePath(LevelDataPath);
	m_ReadWrite.OpenFileToRead();

	bool levelBits[800]{0};

	for (unsigned int i = 0; i < amountOfLevels; i++)
	{
		cp::GameObject* levelGameObject = new cp::GameObject(cp::GameObjectType::level);
		pScene->Add(levelGameObject);

		(i == 0) ? levelGameObject->SetActive(true) : levelGameObject->SetActive(false);

		cp::Transform* levelTransform = levelGameObject->GetComponent<cp::Transform>(cp::ComponentType::_Transform);
		// add a height offset 
		(levelTransform != nullptr) ? levelTransform->SetPosition(0.f, -float(i * m_WindowTileSize * m_LevelTilesHigh), 0.f) : nullptr;

		SDL_Texture* blockTexture = cp::ResourceManager::GetInstance().LoadSDLTexture("LevelData/LevelBlocks.png");

		// source rect of each level block
		SDL_Rect src{};
		src.x = (i % 10) * heightAndWithOfTile;
		src.y = (i / 10) * heightAndWithOfTile;
		src.w = heightAndWithOfTile;
		src.h = heightAndWithOfTile;

		for (unsigned int j = 0; j < bytesPerLevel; j++)
		{
			// base destination rect for each byte in a level
			SDL_FRect dst{};
			dst.x = float((j % (m_LevelTilesWide / bitsInByte)) * m_WindowTileSize * bitsInByte);
			dst.y = float((j / (m_LevelTilesWide / bitsInByte)) * m_WindowTileSize);
			dst.w = float(m_WindowTileSize);
			dst.h = float(m_WindowTileSize);

			unsigned char byte;
			m_ReadWrite.BinaryReading(byte);
			unsigned char mask = 0b10000000;
			for (unsigned int k = 0; k < bitsInByte; k++)
			{
				if (byte & mask) // when the mask returns more than 0 we have a valid level block
				{
					// create the block and add it to the game object
					cp::Texture2D* block = new cp::Texture2D(blockTexture);
					block->SetSrcRect(src);
					block->SetDstRect(dst);
					levelGameObject->AddComponent(block);
					levelBits[(j * 8) + k] = true;
				}
				else
				{
					levelBits[(j * 8) + k] = false;
				}
				mask = mask >> 1;
				dst.x += m_WindowTileSize;
			}
		}
		CalculateLevelCollision(levelGameObject, levelBits);
	}
	m_ReadWrite.CloseFileToRead();
}

void Game::BubbleBobbleLevelReader::CalculateLevelCollision(cp::GameObject* pLevelObj,const bool levelBlocks[800])
{
	std::vector<glm::tvec2<int>> collisionIndicies;
	int cISize{0};

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
					pLevelObj->AddComponent(new cp::CollisionBox(x, y, width, height, side));
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
			pLevelObj->AddComponent(new cp::CollisionBox(x, y, width, height, side));
			collisionIndicies.clear();
			cISize = 0;
		}
	}

	// read up collision 
	// if block ontop of current is false ( valid up collision ) push back collision vector with a pos
	// read the next one RIGHT it ( level data[0] = top left )
	// when the last one in the collumn is read create a collision box component (if size > 0) 
	// OR
	// when the next block in the row is not a valid create a collision box component
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
					pLevelObj->AddComponent(new cp::CollisionBox(x, y, width, height, side));
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
			pLevelObj->AddComponent(new cp::CollisionBox(x, y, width, height, side));
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
					pLevelObj->AddComponent(new cp::CollisionBox(x, y, width, height, side));
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
			pLevelObj->AddComponent(new cp::CollisionBox(x, y, width, height, side));
			collisionIndicies.clear();
			cISize = 0;
		}
	}
}