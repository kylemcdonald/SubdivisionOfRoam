#include "SoundManager.h"

bool SoundManager::enabled = true;

vector<ofSoundPlayer*>
	SoundManager::ripping,
	SoundManager::squawking;
	
void SoundManager::loadSounds(vector<ofSoundPlayer*>& players,	string path) {
	ofLog(OF_LOG_VERBOSE, "Loading sounds from " + path);
	ofxDirList dir;
	int n = dir.listDir(path);
	for(int i = 0; i < n; i++) {
		ofSoundPlayer* cur = new ofSoundPlayer();
		cur->loadSound(dir.getPath(i));
		cur->setMultiPlay(true);
		players.push_back(cur);
	}
}

void SoundManager::setup() {
	loadSounds(ripping, "sound/ripping");
	loadSounds(squawking, "sound/squawking");
}

void SoundManager::randomPlay(vector<ofSoundPlayer*>& players) {
	if(enabled) {
		players[rand() % players.size()]->play();
	}
}

void SoundManager::setVolume(vector<ofSoundPlayer*>& players, float volume) {
	for(int i = 0; i < players.size(); i++) {
		players[i]->setVolume(volume);
	}
}

void SoundManager::randomRipping() {
	randomPlay(ripping);
}

void SoundManager::randomSquawking() {
	randomPlay(squawking);
}

void SoundManager::setRippingVolume(float volume) {
	setVolume(ripping, volume);
}

void SoundManager::setSquawkingVolume(float volume) {
	setVolume(squawking, volume);
}
