#pragma once
#include "AudioLocator.h"
#include <unordered_map>
#include <SDL_mixer.h>

//https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer_11.html
#define FREQUENCY 44100 // sounds better than the default 22050 ( recomended by a friend that is into audio )
#define CHUNKSIZE 1024  // bytes per output sample
#define SFX_CHANNEL -1

namespace Game
{

	class BubbleBobbleAudio final : public cp::Audio
	{
	public:
		BubbleBobbleAudio();
		~BubbleBobbleAudio();

		BubbleBobbleAudio(const BubbleBobbleAudio& other) = default;
		BubbleBobbleAudio(BubbleBobbleAudio&& other) = default;
		BubbleBobbleAudio& operator=(const BubbleBobbleAudio& other) = default;
		BubbleBobbleAudio& operator=(BubbleBobbleAudio&& other) = default;

		enum class Sfx : int
		{
			Jump = 0,
			BalloonJump = 1,
			Catch = 2,
			WellDone = 3,
		};

		enum class Music : int
		{
			GameLoop = 0,
			Intro = 1,
			GameOver = 2,
		};

		typedef std::unordered_map<Sfx, Mix_Chunk*> SFXMap;
		typedef std::unordered_map<Music, Mix_Music*> MusicMap;

		void OpenAudio();
		void Cleanup();

		void AddSfx(Sfx sfx, const std::string& source);
		void AddMusic(Music music, const std::string& source);

		virtual void PlayMusic(int musicId) override;
		virtual void StopMusic(int musicId) override;
		virtual void PlaySfx(int sfxId) override;
		virtual void StopSfx(int sfxId) override;
		virtual void SetMusicVolume(int volume)override;
		virtual void SetSfxVolume(int volume)override;
		virtual void StopAllMusic() override;
		virtual void StopAllSfx() override;

	private:
		bool m_IsPlayingMusic = false;

		MusicMap m_pMusicMap;
		SFXMap m_pSfxMap;
	};

}