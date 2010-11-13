#pragma once

#include "cv.h"
#include "ofMain.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	ofImage img;
};
