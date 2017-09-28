#include "Graphics\Audio.h"

AudioClip::AudioClip(const char *source) {
	mSource = source;
	mpMusic = Mix_LoadMUS(mSource);
}

AudioClip::~AudioClip() {
	this->stop();
	Mix_FreeMusic(mpMusic);
	mpMusic = NULL;
}

void AudioClip::play() {
	if (Mix_PlayingMusic() == 0)
	{
		// music is not playing
		Mix_PlayMusic(mpMusic, 1); // with loop
	}
}

void AudioClip::pause() {
	if (Mix_PlayingMusic() != 0)
	{
		// music is playing
		// If the music is paused
		if (Mix_PausedMusic() == 1)
		{
			//Resume the music
			Mix_ResumeMusic();
		}
		//If the music is playing
		else
		{
			//Pause the music
			Mix_PauseMusic();
		}
	}
}

void AudioClip::stop() {
	if (Mix_PlayingMusic() != 0)
	{
		// music is playing
		// stop music
		Mix_HaltMusic();
	}
}

void AudioClip::restart() {
	this->stop();
	this->play();
}
