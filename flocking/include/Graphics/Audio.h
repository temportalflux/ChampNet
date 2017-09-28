#pragma once

#include <Trackable.h>
#include <SDL_mixer.h>

class AudioClip : public Trackable {

private:
	Mix_Music *mpMusic;
	const char *mSource;

public:
	AudioClip(const char *source);
	virtual ~AudioClip();

	void play();
	void pause();
	void stop();
	void restart();

};
