#include "LoopTexture.h"

float LoopTexture::rate, LoopTexture::overallVolume;

void LoopTexture::setup(string path) {
	ofxDirList dir;
	int n = dir.listDir(path);
	for(int i = 0; i < n; i++) {
		ofSoundPlayer* cur = new ofSoundPlayer();
		cur->loadSound(dir.getPath(i));
		cur->setLoop(true);
		cur->play();
		loops.push_back(cur);
		noiseOffset.push_back(ofRandomuf() * 1024 * 1024);
	}
}

LoopTexture::~LoopTexture() {
	for(int i = 0; i < loops.size(); i++) {
		delete loops[i];
	}
}

void LoopTexture::update() const {
	float curPosition = sin(ofGetElapsedTimef()) * rate;
	for(int i = 0; i < loops.size(); i++) {
		float curVolume = overallVolume * ofNoise(curPosition + noiseOffset[i]);
		loops[i]->setVolume(curVolume);
		if(!loops[i]->getIsPlaying())
			loops[i]->play();
	}
}