#include "HoleManager.h"

vector<ofImage*> HoleManager::holeImages;
vector<Hole> HoleManager::holes;
float HoleManager::holeSpacing;

void HoleManager::setup() {
	ofxDirList dir;
	int n = dir.listDir("holes");
	for(int i = 0; i < n; i++) {
		ofImage* cur = new ofImage();
		cur->loadImage(dir.getPath(i));
		 // holes need to be drawn small and fast
		cur->resize(cur->getWidth() / 5, cur->getHeight() / 5);
		holeImages.push_back(cur);
	}
}

ofImage* HoleManager::randomHoleImage() {
	return holeImages[(int) ofRandom(0, holeImages.size())];
}

void HoleManager::update() {
	for(int i = 0; i < holes.size(); i++) {
		holes[i].update();
	}
}

void HoleManager::draw() {
	for(int i = 0; i < holes.size(); i++) {
		holes[i].draw();
	}
}

void HoleManager::add(ofPoint& point) {
	holes.push_back(Hole());
	holes.back().setup(point);
}

bool HoleManager::intersects(ofPoint& target) {
	for(int i = 0; i < holes.size(); i++) {
		if(holes[i].distance(target) < HoleManager::holeSpacing) {
			return true;
		}
	}
	return false;
}