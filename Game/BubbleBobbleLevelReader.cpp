#include "CodePainPCH.h"
#include "BubbleBobbleLevelReader.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Components.h"

void Game::BubbleBobbleLevelReader::ReadLevelData(cp::Scene* pScene)
{
	const std::string LevelDataPath = cp::ResourceManager::GetInstance().GetDataPath() + "LevelData/SeperatedLevelData.dat";

	const unsigned int amountOfLevels = 100;
	const unsigned int levelTilesWide = 32;
	const unsigned int levelTilesHigh = 25;
	const unsigned int bitsInByte = 8;
	const unsigned int bytesPerLevel = (levelTilesWide * levelTilesHigh) / bitsInByte;
	const unsigned int heightAndWithOfTile = 8;

	const unsigned int windowSizeX = 640;
	const unsigned int windowSizeY = 500;
	const unsigned int windowTileSizeX = windowSizeX / levelTilesWide;
	const unsigned int windowTileSizeY = windowSizeY / levelTilesHigh;

	m_ReadWrite.ChangeFilePath(LevelDataPath);

	m_ReadWrite.OpenFileToRead();
	for (unsigned int i = 0; i < amountOfLevels; i++)
	{
		cp::GameObject* levelGameObject = new cp::GameObject(cp::GameObjectType::level);
		pScene->Add(levelGameObject);

		(i == 0) ? levelGameObject->SetActive(true) : levelGameObject->SetActive(false);

		cp::Transform* levelTransform = levelGameObject->GetComponent<cp::Transform>(cp::ComponentType::_Transform);
		// add a height offset 
		(levelTransform != nullptr) ? levelTransform->SetPosition(0.f, -float(i * windowTileSizeY * levelTilesHigh), 0.f) : nullptr;

		SDL_Texture* blockTexture = cp::ResourceManager::GetInstance().LoadSDLTexture("LevelData/LevelBlocks.png");

		// source rect of each level block
		SDL_Rect src{};
		src.x = (i % 10) * 8;
		src.y = (i / 10) * 8;
		src.w = heightAndWithOfTile;
		src.h = heightAndWithOfTile;

		for (unsigned int j = 0; j < bytesPerLevel; j++)
		{
			// base destination rect for each byte in a level
			SDL_FRect dst{};
			dst.x = float((j % (levelTilesWide / bitsInByte)) * windowTileSizeX * bitsInByte);
			dst.y = float((j / (levelTilesWide / bitsInByte)) * windowTileSizeY);
			dst.w = float(windowTileSizeX);
			dst.h = float(windowTileSizeY);

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
				}
				mask = mask >> 1;
				dst.x += windowTileSizeX;
			}
		}
	}
	m_ReadWrite.CloseFileToRead();
}
