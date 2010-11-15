#pragma once

#include "ofxDirList.h"

class Animation {
protected:
	vector<ofImage*> frames;
public:
	Animation() {
	}
	void setup(string directory) {
		ofLog(OF_LOG_VERBOSE, "Loading animation from " + directory);
		ofxDirList dir;
		int n = dir.listDir(directory);
		for(int i = 0; i < n; i++) {
			ofImage* cur = new ofImage();
			cur->loadImage(dir.getPath(i));
			cur->setAnchorPercent(.5, .5);
			// if possible, set GL type to just alpha
			frames.push_back(cur);
		}
	}
	~Animation() {
		for(int i = 0; i < frames.size(); i++) {
			delete frames[i];
		}
	}
	void draw(int i) {
		frames[i % frames.size()]->draw(0, 0);
	}
};