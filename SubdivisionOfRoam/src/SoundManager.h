#pragma once

#include "ofxDirList.h"

class SoundManager {
protected:
	static vector<ofSoundPlayer*> ambience, attacking, flapping, ripping, squawking, gliding;
	static void loadSounds(vector<ofSoundPlayer*>& players,	string path);
	static void randomPlay(vector<ofSoundPlayer*>& players);
	static void setVolume(vector<ofSoundPlayer*>& players, float volume);
public:
	static void setup();
	
	static void randomAmbience();
	static void randomAttacking();
	static void randomFlapping();
	static void randomRipping();
	static void randomSquawking();
	static void randomGliding();
	
	static void setAmbienceVolume(float volume);
	static void setAttackingVolume(float volume);
	static void setFlappingVolume(float volume);
	static void setRippingVolume(float volume);
	static void setSquawkingVolume(float volume);
	static void setGlidingVolume(float volume);
	 
	static bool enabled;
};