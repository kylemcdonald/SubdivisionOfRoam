#pragma once

#include "ofMain.h"
#include "ofxBlur.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	static const int camWidth = 640;
	static const int camHeight = 480;
	
	ofVideoGrabber camera;
	ofxBlur blur;
};
