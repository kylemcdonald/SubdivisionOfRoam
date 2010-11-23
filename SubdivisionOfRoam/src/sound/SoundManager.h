#pragma once

#include "ofxDirList.h"

class SoundManager {
protected:
	static vector<ofSoundPlayer*> ripping, squawking;
	static void loadSounds(vector<ofSoundPlayer*>& players,	string path);
	static void randomPlay(vector<ofSoundPlayer*>& players);
	static void setVolume(vector<ofSoundPlayer*>& players, float volume);
public:
	static void setup();
	
	static void randomRipping();
	static void randomSquawking();
	
	static void setRippingVolume(float volume);
	static void setSquawkingVolume(float volume);
	 
	static bool enabled;
};