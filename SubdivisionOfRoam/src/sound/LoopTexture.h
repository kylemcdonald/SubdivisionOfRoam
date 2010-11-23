#pragma once

#include "ofMain.h"
#include "ofxDirList.h"

class LoopTexture {
private:
	vector<ofSoundPlayer*> loops;
	vector<float> noiseOffset;
public:
	static float rate, overallVolume;
	
	void setup(string path);
	~LoopTexture();
	void update() const;
};