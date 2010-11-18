#pragma once

#include "ofxDirList.h"

class SoundManager {
protected:
	static vector<ofSoundPlayer*> ambience, attacking, flapping, ripping, squawking;
	static void loadSounds(vector<ofSoundPlayer*>& players,	string path);
	static void randomPlay(vector<ofSoundPlayer*>& players);
public:
	static void setup();
	static void randomFlapping();
	static void randomSquawking();
	static void randomRipping();
	
	static bool enabled;
};