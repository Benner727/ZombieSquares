#include "AudioManager.h"

AudioManager* AudioManager::sInstance = nullptr;

AudioManager* AudioManager::Instance()
{
	if (sInstance == nullptr)
		sInstance = new AudioManager();

	return sInstance;
}

void AudioManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

AudioManager::AudioManager()
{
	mAssetManager = AssetManager::Instance();

	int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	if (!Mix_Init(flags) & flags) 
		std::cout << "Mixer Initialization Error: " << Mix_GetError() << std::endl;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
		std::cout << "Mixer Initialization Error: " << Mix_GetError() << std::endl;

	Volume(100);
}

AudioManager::~AudioManager()
{
	mAssetManager = nullptr;
	Mix_Quit();
}

void AudioManager::PlayMusic(std::string filename, int loops)
{
	Mix_PlayMusic(mAssetManager->GetMusic("Audio/" + filename), loops);
}

void AudioManager::PauseMusic()
{
	if (Mix_PlayingMusic() != 0)
		Mix_PauseMusic();
}

void AudioManager::ResumeMusic()
{
	if (Mix_PausedMusic() != 0)
		Mix_ResumeMusic();
}

void AudioManager::PlaySFX(std::string filename, int loops, int channel, bool playover)
{
	if (playover || !Mix_Playing(channel))
		Mix_PlayChannel(channel, mAssetManager->GetSFX("Audio/" + filename), loops);
}

void AudioManager::StopSFX(int channel)
{
	Mix_HaltChannel(channel);
}

int AudioManager::Volume() const
{
	return mVolume;
}

void AudioManager::Volume(int volume)
{
	if (volume > 100 || volume < 0)
		return;

	mVolume = volume;

	Mix_VolumeMusic(MIX_MAX_VOLUME / 5.0f * (float)(mVolume / 100.0f)); //Music Volume
	Mix_Volume(1, MIX_MAX_VOLUME / 10.0f * (float)(mVolume / 100.0f)); //Player shooting
	Mix_Volume(2, MIX_MAX_VOLUME / 4.0f * (float)(mVolume / 100.0f)); //Player reloading
	Mix_Volume(3, MIX_MAX_VOLUME / 5.0f * (float)(mVolume / 100.0f)); //Player death
	Mix_Volume(4, MIX_MAX_VOLUME / 5.0f * (float)(mVolume / 100.0f)); //Zombie attack
	Mix_Volume(5, MIX_MAX_VOLUME / 5.0f * (float)(mVolume / 100.0f)); //Zombie death
}
