#pragma once

#include "ofxDirList.h"

class Animation {
protected:
	vector<ofImage*> frames;
public:
	Animation() {
	}
	void setup(string directory) {
		ofxDirList dir;
		int n = dir.listDir(directory);
		for(int i = 0; i < n; i++) {
			ofImage* cur = new ofImage();
			cur->loadImage(dir.getPath(i));
			cur->setAnchorPercent(.5, .5);
			// if possible, set GL type to just alpha?
			frames.push_back(cur);
		}
	}
	float getPercent(float i) {
		float cur = fmodf(i, frames.size());
		return cur / frames.size();
	}
	~Animation() {
		for(int i = 0; i < frames.size(); i++) {
			delete frames[i];
		}
	}
	bool draw(int i) {
		int cur = i % frames.size();
		frames[cur]->draw(0, 0);
		return cur == frames.size() - 1;
	}
	bool drawPercent(float percent) {
		int i = ofClamp(percent * frames.size(), 0, frames.size() - 1);
		return draw(i);
	}
};