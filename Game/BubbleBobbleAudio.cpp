#include "CodePainPCH.h"
#include "BubbleBobbleAudio.h"
#include "Logger.h"
#include "ResourceManager.h"

Game::BubbleBobbleAudio::BubbleBobbleAudio()
	:cp::Audio()
{
	OpenAudio();
}

Game::BubbleBobbleAudio::~BubbleBobbleAudio()
{
	Cleanup();
}

void Game::BubbleBobbleAudio::OpenAudio()
{
	if (Mix_OpenAudio(FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS, CHUNKSIZE) < 0)
		cp::Logger::GetInstance().Log(cp::LogLevel::Critical, Mix_GetError());
}

void Game::BubbleBobbleAudio::Cleanup()
{
	for (std::pair<Sfx, Mix_Chunk*> toCleanup : m_pSfxMap)
		Mix_FreeChunk(toCleanup.second);
	m_pSfxMap.clear();

	for (std::pair<Music, Mix_Music*> toCleanup : m_pMusicMap)
		Mix_FreeMusic(toCleanup.second);
	m_pMusicMap.clear();

	cp::Logger::GetInstance().Log(cp::LogLevel::Info, "Sound system cleaned!");
}

// can load WAVE, AIFF, RIFF, OGG, VOC
// source = dataFolder/...source
void Game::BubbleBobbleAudio::AddSfx(Sfx sfx, const std::string& source)
{
	std::string path = cp::ResourceManager::GetInstance().GetDataPath() + source;
	Mix_Chunk* mixSfx = Mix_LoadWAV(path.c_str());
	if (mixSfx != nullptr)
		m_pSfxMap[sfx] = mixSfx;
	else
		cp::Logger::GetInstance().Log(cp::LogLevel::Error, Mix_GetError());
}

// can load WAVE, MOD, MIDI, OGG, MP3, FLAC
// source = dataFolder/...source
void Game::BubbleBobbleAudio::AddMusic(Music music, const std::string& source)
{
	std::string path = cp::ResourceManager::GetInstance().GetDataPath() + source;
	Mix_Music* mixMusic = Mix_LoadMUS(path.c_str());
	if (mixMusic != nullptr)
		m_pMusicMap[music] = mixMusic;
	else
		cp::Logger::GetInstance().Log(cp::LogLevel::Error, Mix_GetError());
}

// only 1 song is allowed to play at once 
// ( design choice ) no need for multiple songs ontop of eachoter
void Game::BubbleBobbleAudio::PlayMusic(int musicId)
{
	Music music = Music(musicId);
	switch (music)
	{
	case Game::BubbleBobbleAudio::Music::GameLoop:
		StopMusic(musicId);
		if (Mix_PlayMusic(m_pMusicMap[music], -1) == -1)
			cp::Logger::GetInstance().Log(cp::LogLevel::Error, Mix_GetError());
		break;
	case Game::BubbleBobbleAudio::Music::Intro:
	case Game::BubbleBobbleAudio::Music::GameOver:
		StopMusic(musicId);
		if (Mix_PlayMusic(m_pMusicMap[music], 1) == -1)
			cp::Logger::GetInstance().Log(cp::LogLevel::Error, Mix_GetError());
		break;
	default:
		cp::Logger::GetInstance().Log(cp::LogLevel::Warning, "Music id: " + std::to_string(musicId) + " does not exitst!");
		break;
	}
}

// checks if you are giving a valid music channel 
// ( will stop all music but since only 1 music song can be played no issue )
void Game::BubbleBobbleAudio::StopMusic(int musicId)
{
	Music music = Music(musicId);
	switch (music)
	{
	case Game::BubbleBobbleAudio::Music::GameLoop:
	case Game::BubbleBobbleAudio::Music::Intro:
	case Game::BubbleBobbleAudio::Music::GameOver:
		Mix_HaltMusic(); // no need to check as it always returns 0
		break;
	default:
		break;
	}
}

void Game::BubbleBobbleAudio::PlaySfx(int sfxId)
{
	Sfx effect = Sfx(sfxId);
	switch (effect)
	{
	case Game::BubbleBobbleAudio::Sfx::Jump:
	case Game::BubbleBobbleAudio::Sfx::BalloonJump:
	case Game::BubbleBobbleAudio::Sfx::Catch:
	case Game::BubbleBobbleAudio::Sfx::WellDone:
		if (Mix_PlayChannel(SFX_CHANNEL, m_pSfxMap[effect], 0) == -1)
			cp::Logger::GetInstance().Log(cp::LogLevel::Error, Mix_GetError());
		break;
	default:
		cp::Logger::GetInstance().Log(cp::LogLevel::Warning, "Sfx id: " + std::to_string(sfxId) + " does not exitst!");
		break;
	}
}

// checks the sfxId if valid will stop the current sound effects playing
void Game::BubbleBobbleAudio::StopSfx(int sfxId)
{
	switch (Sfx(sfxId))
	{
	case Game::BubbleBobbleAudio::Sfx::Jump:
	case Game::BubbleBobbleAudio::Sfx::BalloonJump:
	case Game::BubbleBobbleAudio::Sfx::Catch:
	case Game::BubbleBobbleAudio::Sfx::WellDone:
		Mix_HaltChannel(SFX_CHANNEL); // no need to check always returns 0
		break;
	default:
		break;
	}
}

void Game::BubbleBobbleAudio::SetMusicVolume(int volume)
{
	Mix_VolumeMusic(volume);
}

void Game::BubbleBobbleAudio::SetSfxVolume(int volume)
{
	for (std::pair<Sfx, Mix_Chunk*> toSet : m_pSfxMap)
		Mix_VolumeChunk(toSet.second, volume);
}

// doesnt check anything and will halt all music
void Game::BubbleBobbleAudio::StopAllMusic()
{
	Mix_HaltMusic(); // no need to check as it always returns 0
}

// doesnt check anything and will halt all sound effects
void Game::BubbleBobbleAudio::StopAllSfx()
{
	Mix_HaltChannel(SFX_CHANNEL); // no need to check always returns 0
}

