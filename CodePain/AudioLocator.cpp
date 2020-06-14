#include "CodePainPCH.h"
#include "AudioLocator.h"

cp::Audio* cp::AudioLocator::m_pAudioService = nullptr;
cp::NullAudio cp::AudioLocator::m_DefaultAudioService = NullAudio{};

void cp::AudioLocator::Init()
{
	m_pAudioService = &m_DefaultAudioService;
}

void cp::AudioLocator::DeleteAudio()
{
	SAFE_DELETE(m_pAudioService);
}

void cp::AudioLocator::RegisterAudioService(Audio* service)
{
	m_pAudioService = (service == nullptr) ? &m_DefaultAudioService : service;
}

void cp::NullAudio::PlayMusic(int)
{
}

void cp::NullAudio::StopMusic(int)
{
}

void cp::NullAudio::PlaySfx(int)
{
}

void cp::NullAudio::StopSfx(int)
{
}

void cp::NullAudio::SetMusicVolume(int)
{
}

void cp::NullAudio::SetSfxVolume(int)
{
}

void cp::NullAudio::StopAllMusic()
{
}

void cp::NullAudio::StopAllSfx()
{
}
