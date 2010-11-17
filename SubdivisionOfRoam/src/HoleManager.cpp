#include "HoleManager.h"

vector<ofImage*> HoleManager::holeImages;

void HoleManager::setup() {
	ofxDirList dir;
	int n = dir.listDir("holes");
	for(int i = 0; i < n; i++) {
		ofImage* cur = new ofImage();
		cur->loadImage(dir.getPath(i));
		holeImages.push_back(cur);
	}
}

ofImage* HoleManager::randomHoleImage() {
	return holeImages[(int) ofRandom(0, holeImages.size())];
}