#include "CodePainPCH.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Observer.h"
#include "SceneManager.h"
#include "Scene.h"
#include <SDL_mixer.h>

cp::GameManager::~GameManager()
{
	SAFE_DELETE(m_ManagerObj);
}

void cp::GameManager::Initialize()
{
	m_ManagerObj = new GameObject();
	m_CurrentLevel = 1;
}

size_t cp::GameManager::GetChannelVolume(int channel) const
{
	if (IsValidChannel(channel - 1))
		return m_ChannelVolume[channel - 1];
	return 0;
}

void cp::GameManager::SetCurrentLevel(size_t currentLevel)
{
	m_CurrentLevel = currentLevel;

	m_ManagerObj->NotifyObservers(cp::Event::EVENT_SPAWN_ENEMIES);
}

void cp::GameManager::SetChannelVolume(int channel, size_t volume)
{
	int chnlVolume = int(float(volume / 100.f) * float(MIX_MAX_VOLUME));

	if (channel == 1)
	{
		m_ChannelVolume[0] = volume;
		cp::AudioLocator::GetAudio()->SetSfxVolume(chnlVolume);
	}
	if (channel == 2)
	{
		m_ChannelVolume[1] = volume;
		cp::AudioLocator::GetAudio()->SetMusicVolume(chnlVolume);
	}
	else if (channel == -1)
	{
		m_ChannelVolume[0] = volume;
		m_ChannelVolume[1] = volume;
		cp::AudioLocator::GetAudio()->SetSfxVolume(chnlVolume);
		cp::AudioLocator::GetAudio()->SetMusicVolume(chnlVolume);
	}
}

bool cp::GameManager::IsValidChannel(int channel) const
{
	return (channel >= 0 && channel < CHANNELS);
}
