#pragma once
#define CHANNELS 2 // sterio
#define STARTVOLUME 20

namespace cp
{

    class Audio abstract
    {
    public:
        Audio() = default;
        virtual ~Audio() = default;

       Audio(const Audio& other) = default;
       Audio(Audio&& other) = default;
       Audio& operator=(const Audio& other) = default;
       Audio& operator=(Audio&& other) = default;

       virtual void PlayMusic(int musicId) = 0;
       virtual void StopMusic(int musicId) = 0;
       virtual void PlaySfx(int sfxId) = 0;
       virtual void StopSfx(int sfxId) = 0;
       virtual void SetMusicVolume(int volume) = 0;
       virtual void SetSfxVolume(int volume) = 0;
       virtual void StopAllMusic() = 0;
       virtual void StopAllSfx() = 0;
    };

    class NullAudio final : public Audio
    {
    public:
        NullAudio() = default;
        virtual ~NullAudio() = default;

        NullAudio(const NullAudio& other) = default;
        NullAudio(NullAudio&& other) = default;
        NullAudio& operator=(const NullAudio& other) = default;
        NullAudio& operator=(NullAudio&& other) = default;

        virtual void PlayMusic(int musicId) override;
        virtual void StopMusic(int musicId) override;
        virtual void PlaySfx(int sfxId) override;
        virtual void StopSfx(int sfxId) override;
        virtual void SetMusicVolume(int volume)override;
        virtual void SetSfxVolume(int volume)override;
        virtual void StopAllMusic() override;
        virtual void StopAllSfx() override;
    };

    class AudioLocator final
    {
    public:
        AudioLocator() = default;
        ~AudioLocator() = default;

        AudioLocator(const AudioLocator& other) = delete;
        AudioLocator(AudioLocator&& other) = delete;
        AudioLocator& operator=(const AudioLocator& other) = delete;
        AudioLocator& operator=(AudioLocator&& other) = delete;

        static void Init();
        static void DeleteAudio();
        static void RegisterAudioService(Audio* service);

        static Audio* GetAudio() { return m_pAudioService; }

    private:
        static Audio* m_pAudioService;
        static NullAudio m_DefaultAudioService;
    };
}