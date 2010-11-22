#pragma once

#include "ofxVectorMath.h"
#include "AnimationManager.h"

class Debris {
private:
	ofxVec2f position;
	Animation* animation;
public:
	void setup(ofxVec2f position);	
	void draw();
};