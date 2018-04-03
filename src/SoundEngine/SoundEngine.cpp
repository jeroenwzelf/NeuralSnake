#include "SoundEngine.h"

sound_engine::sound_engine() {
	engine = irrklang::createIrrKlangDevice();
}

void sound_engine::play_nom() {
	play_fx("nom" + std::to_string(randomint(1,9)) + ".wav");
}

void sound_engine::play_music() {
	std::string fileName = filePath + "music.wav";
	music = engine->play2D(fileName.c_str(), true, false, true);
}
void sound_engine::stop_music() {
	if (music) {
		music->stop();
		music->drop();
		music = 0; // music->roll() ?
	}
}

void sound_engine::play_fx(const std::string fileName) {
	const std::string file = filePath + fileName;
	engine->play2D(file.c_str(), false, false, true)->setVolume(0.1);
}

sound_engine::~sound_engine() {
	engine->drop();
}