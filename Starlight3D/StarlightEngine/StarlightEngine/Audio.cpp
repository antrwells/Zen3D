#include "pch.h"
#include "Audio.h"

Audio::Audio() {

	if (engine == nullptr) {

		engine = createIrrKlangDevice();

	}

}

void Audio::PlaySong(const char* path)
{
	
	songSound = engine->play2D(path, true);

}

void Audio::EndSong() {
		
	if (engine == nullptr) return;
	engine->drop();
	if (songSound != nullptr) {

		songSound->stop();

	}
	engine = nullptr;
}

ISound* Audio::songSound = nullptr;
ISoundEngine* Audio::engine = nullptr;