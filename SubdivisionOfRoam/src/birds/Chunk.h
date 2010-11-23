#pragma once

#include "ofImage.h"

class Chunk {
private:
	ofImage* leftHole;
	ofImage* rightHole;
	float rotation;
public:
	static float chunkScale, carryDistance;
	
	void setup();
	void draw();
};