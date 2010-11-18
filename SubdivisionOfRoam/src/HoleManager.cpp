#include "HoleManager.h"

vector<ofImage*> HoleManager::holeImages;

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