 #pragma once

#include "structures.h"
#include <irrKlang.h>
#include <string>

class sound_engine {
	public:
		sound_engine();
		void play_nom();

		void play_music();
		void stop_music();
		~sound_engine();
	private:
		const std::string filePath = "src/Sounds/";
		void play_fx(const std::string fileName);
		irrklang::ISound* music;
		irrklang::ISoundEngine* engine;
};