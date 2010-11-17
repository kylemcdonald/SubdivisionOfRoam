#pragma once

#include "ofMain.h"
#include "ofxDirList.h"

class HoleManager {
private:
	static vector<ofImage*> holeImages;
public:
	static void setup();
	static ofImage* randomHoleImage();
};