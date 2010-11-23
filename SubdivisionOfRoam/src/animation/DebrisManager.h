#pragma once

#include "ofxDirList.h"
#include "Debris.h"

class DebrisManager {
private:
	static vector<Debris> debris;
public:
	static void setup();
	static void add(ofPoint position);
	static void update();
	static void draw();
};