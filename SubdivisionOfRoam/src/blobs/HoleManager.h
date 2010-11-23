#pragma once

#include "ofMain.h"
#include "ofxDirList.h"

#include "Hole.h"
class Hole;

class HoleManager {
private:
	static vector<ofImage*> holeImages;
public:
	static vector<Hole> holes;
	static float holeSpacing;
	
	static void setup();
	static ofImage* randomHoleImage();
	static void update();
	static void draw();
	static void add(ofPoint& hole);
	static bool intersects(ofPoint& target);
};