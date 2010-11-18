#pragma once

#include "HoleManager.h"
#include "testApp.h"

class Hole {
protected:	
	ofImage* img;
	ofxVec2f position;
	
	int blobIndex;
	int center;
	float birth;
public:
	Hole();
	void setup(ofxVec2f position);
	void update();
	void draw();
	float distance(ofPoint& point);
	float getAge();
	
	static int holeRadius;
	static float maxHoleAge;
};