#include "CodePainPCH.h"
#include "LevelScene.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "BubbleBobbleDataReader.h"
#include "GameObject.h"
#include "Logger.h"
#include "Components.h"
#include "InputHandler.h"
#include "InputCommand.h"
#include "Commands.h"
#include "States.h"
#include "Observers.h"
#include "GameManager.h"
#include "AudioLocator.h"
#include "BubbleBobbleAudio.h"

void Game::LevelScene::LoadSceneData() const
{
	cp::Scene* scene = cp::SceneManager::GetInstance().CreateScene("LevelScene");
	BubbleBobbleDataReader& dataReader = BubbleBobbleDataReader::GetInstance();
	dataReader.ReadLevelData(scene, "LevelData/SeperatedLevelData.dat", "LevelData/LevelParallaxColors.png");
	dataReader.ReadEnemyData("CharacterData/SeperatedEnemyData.dat");

#pragma region AudioInitialization

	Game::BubbleBobbleAudio* bubbleBobbleAudio = new Game::BubbleBobbleAudio();
	bubbleBobbleAudio->AddMusic(Game::BubbleBobbleAudio::Music::GameLoop, "Audio/gameloop.wav");
	// mp3 test file ;) (note loud)
	// bubbleBobbleAudio->AddMusic(Game::BubbleBobbleAudio::Music::GameLoop, "Audio/bruhblebrohblemaintheme.mp3");
	bubbleBobbleAudio->AddMusic(Game::BubbleBobbleAudio::Music::GameOver, "Audio/gameover.wav");
	bubbleBobbleAudio->AddMusic(Game::BubbleBobbleAudio::Music::Intro, "Audio/intro.wav");
	bubbleBobbleAudio->SetMusicVolume(STARTVOLUME);

	bubbleBobbleAudio->AddSfx(Game::BubbleBobbleAudio::Sfx::Jump, "Audio/jump.wav");
	bubbleBobbleAudio->AddSfx(Game::BubbleBobbleAudio::Sfx::BalloonJump, "Audio/balloonjump.wav");
	bubbleBobbleAudio->AddSfx(Game::BubbleBobbleAudio::Sfx::Catch, "Audio/catch.wav");
	bubbleBobbleAudio->AddSfx(Game::BubbleBobbleAudio::Sfx::WellDone, "Audio/welldone.wav");
	bubbleBobbleAudio->SetSfxVolume(STARTVOLUME);
	cp::AudioLocator::RegisterAudioService(bubbleBobbleAudio);
	cp::AudioLocator::GetAudio()->PlayMusic((int)Game::BubbleBobbleAudio::Music::GameLoop);

#pragma endregion

#pragma region enemySpawner

	{
		cp::GameManager& gameManager = cp::GameManager::GetInstance();
		gameManager.GetManagerObj()->AddObserver(new Game::SpawnLevelEnemies());
	}

#pragma endregion

	cp::GameObject* p1 = new cp::GameObject(cp::GameObjectType::Player1);
	scene->Add(p1);
	cp::Command* start1PCommand = new Game::StartP1Game();
	cp::Command* start2PCommand = new Game::StartP2Game();
	cp::Command* start1V1Command = new Game::Start1v1Game();
	cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::Key1, cp::InputState::Hold, start1PCommand);
	cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::Key2, cp::InputState::Hold, start2PCommand);
	cp::InputHandler::GetInstance().AddInput(cp::KeyboardKey::Key3, cp::InputState::Hold, start1V1Command);

	cp::GameObject* p2 = new cp::GameObject(cp::GameObjectType::Player2);
	scene->Add(p2);
	{
		cp::GameObject* startMenu = new cp::GameObject(cp::GameObjectType::UIElements);
		cp::Texture2D* BubbleBobbleText = new cp::Texture2D(cp::ResourceManager::GetInstance().LoadSDLTexture("UI/Modes.png"));
		BubbleBobbleText->SetDstRect({ 175,20,300,160});
		startMenu->AddComponent(BubbleBobbleText);
		startMenu->SetActive(true);
		scene->Add(startMenu); 
	}

	{
		cp::GameObject* bubbleBobbleStatistics = new cp::GameObject(cp::GameObjectType::UIElements);

		cp::Texture2D* BubbleBobbleText = new cp::Texture2D(cp::ResourceManager::GetInstance().LoadSDLTexture("UI/BubbleBobble.png"));
		BubbleBobbleText->SetDstRect({ 652,0,200,90 });
		bubbleBobbleStatistics->AddComponent(BubbleBobbleText);

		cp::Texture2D* bubbleBobbleP1 = new cp::Texture2D(cp::ResourceManager::GetInstance().LoadSDLTexture("UI/InfoPlayers.png"));
		bubbleBobbleP1->SetSrcRect({ 0,0,64,16 });
		bubbleBobbleP1->SetDstRect({ 652,150,200,50 });
		bubbleBobbleStatistics->AddComponent(bubbleBobbleP1);

		cp::Text* bubbleBobbleP1Lives = new cp::Text("0 HP", cp::ResourceManager::GetInstance().LoadFont("Font/BubbleBobble.otf", 40), { 0,204,0 });
		bubbleBobbleP1Lives->GetTexture2D()->AddLocalOffset(652, 200);
		bubbleBobbleStatistics->AddComponent(bubbleBobbleP1Lives);

		cp::Texture2D* bubbleBobbleP2 = new cp::Texture2D(cp::ResourceManager::GetInstance().LoadSDLTexture("UI/InfoPlayers.png"));
		bubbleBobbleP2->SetSrcRect({ 0,16,64,16 });
		bubbleBobbleP2->SetDstRect({ 652,300,200,50 });
		bubbleBobbleStatistics->AddComponent(bubbleBobbleP2);

		cp::Text* bubbleBobbleP2Lives = new cp::Text("0 HP", cp::ResourceManager::GetInstance().LoadFont("Font/BubbleBobble.otf", 40), { 68,170,221 });
		bubbleBobbleP2Lives->GetTexture2D()->AddLocalOffset(652, 350);
		bubbleBobbleStatistics->AddComponent(bubbleBobbleP2Lives);

		Uint32 levelSquareColor{};
		levelSquareColor = (levelSquareColor | 255) << 8;
		levelSquareColor = (levelSquareColor | 0) << 8;
		levelSquareColor = (levelSquareColor | 0) << 8;
		levelSquareColor = (levelSquareColor | 0) << 0;
		cp::ColorRect2D* bubbleBobbleLevelSquare = new cp::ColorRect2D(0, 0, 40, 20, levelSquareColor);
		bubbleBobbleStatistics->AddComponent(bubbleBobbleLevelSquare);

		cp::Text* levelText = new cp::Text(" 01", cp::ResourceManager::GetInstance().LoadFont("Font/BubbleBobble.otf", 25), { 255,255,255 });
		levelText->GetTexture2D()->AddLocalOffset(0, -5);
		bubbleBobbleStatistics->AddComponent(levelText);
		scene->Add(bubbleBobbleStatistics);

		cp::Text* bubbleBobbleP1Score = new cp::Text("0", cp::ResourceManager::GetInstance().LoadFont("Font/BubbleBobble.otf", 40), { 0,204,0 });
		bubbleBobbleP1Score->GetTexture2D()->AddLocalOffset(652, 250);
		bubbleBobbleStatistics->AddComponent(bubbleBobbleP1Score);

		cp::Text* bubbleBobbleP2Score = new cp::Text("0", cp::ResourceManager::GetInstance().LoadFont("Font/BubbleBobble.otf", 40), { 68,170,221 });
		bubbleBobbleP2Score->GetTexture2D()->AddLocalOffset(652, 400);
		bubbleBobbleStatistics->AddComponent(bubbleBobbleP2Score);
	}

#pragma region ExtraLevelCollision

	std::vector<cp::GameObject*> levelObjects = scene->GetAllGameObjectsOfType(cp::GameObjectType::level);

	for (size_t i = 0; i < levelObjects.size(); i++)
	{
		levelObjects[i]->AddComponent(new cp::CollisionBox(0, -500, 2 * 20, 20 * 25 + 2 * 500, cp::CollisionBox::CollisionSide::right, cp::CollisionBox::CollisionType::_static));
		levelObjects[i]->AddComponent(new cp::CollisionBox((20 * 30), -500, 2 * 20, 20 * 25 + 2 * 500, cp::CollisionBox::CollisionSide::left, cp::CollisionBox::CollisionType::_static));
		levelObjects[i]->AddComponent(new cp::CollisionBox(40, 550, 560, 30, cp::CollisionBox::CollisionSide::all, cp::CollisionBox::CollisionType::_dynamic));
		levelObjects[i]->AddComponent(new cp::CollisionBox(40, -20, 560, 30, cp::CollisionBox::CollisionSide::all, cp::CollisionBox::CollisionType::_dynamic));
	}

#pragma endregion

	cp::GameManager::GetInstance().GetManagerObj()->AddObserver(new ProceedToNextLevel());

}
