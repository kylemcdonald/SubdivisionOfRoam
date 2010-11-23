#pragma once

#include "ofImage.h"

class Chunk {
private:
	ofImage* leftHole;
	ofImage* rightHole;
public:
	void setup();
	void draw();
};