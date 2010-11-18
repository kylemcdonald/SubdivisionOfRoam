#pragma once

#include "HoleManager.h"
#include "testApp.h"

class Hole {
protected:
	ofImage* img;
	ofxVec2f position;
	
	int blobIndex;
	int center;
	int movement;
public:
	Hole();
	void setup(ofxVec2f position);
	void update();
	void draw();
};