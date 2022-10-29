#pragma once
#include "irrKlang.h"
using namespace irrklang;
class Audio
{
public:
	Audio();
	static void PlaySong(const char* path);
	static void EndSong();
	static ISoundEngine* engine;
	static ISound* songSound;
};

