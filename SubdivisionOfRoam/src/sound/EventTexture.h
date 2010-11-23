#pragma once

#include "ofMain.h"
#include "ofxDirList.h"

class EventTexture {
private:
	vector<ofSoundPlayer*> events;
public:
	static float density;
	
	void setup(string path);
	~EventTexture();
	void randomEvent();
	void update();
	void setVolume(float volume);
};