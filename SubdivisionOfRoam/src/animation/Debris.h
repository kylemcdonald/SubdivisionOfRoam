#pragma once

#include "ofxVectorMath.h"
#include "AnimationManager.h"

class Debris {
private:
	ofxVec2f position;
	float rotation;
	Animation* animation;
	float birth;
public:
	static float maxAge;
	
	void setup(ofxVec2f position);	
	void draw();
	float getAge();
};