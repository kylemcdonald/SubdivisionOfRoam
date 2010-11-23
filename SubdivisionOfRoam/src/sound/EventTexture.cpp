#include "EventTexture.h"

float EventTexture::density;

void EventTexture::setup(string path) {
	ofxDirList dir;
	int n = dir.listDir(path);
	for(int i = 0; i < n; i++) {
		ofSoundPlayer* cur = new ofSoundPlayer();
		cur->loadSound(dir.getPath(i));
		cur->setMultiPlay(true);
		events.push_back(cur);
	}
}

EventTexture::~EventTexture() {
	for(int i = 0; i < events.size(); i++) {
		delete events[i];
	}
}

void EventTexture::update() {
	for(int i = 0; i < events.size(); i++) {
		if(ofRandomuf() < ofGetLastFrameTime() * density) {
			randomEvent();
		}
	}
}

void EventTexture::randomEvent() {
	events[rand() % events.size()]->play();
}

void EventTexture::setVolume(float volume) {
	for(int i = 0; i < events.size(); i++) {
		events[i]->setVolume(volume);
	}
}
