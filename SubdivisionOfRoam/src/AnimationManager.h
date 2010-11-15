#pragma once

#include "Animation.h"

class AnimationManager {
	vector<Animation*> flocking;
	vector<Animation*> attacking;
public:
	AnimationManager() {
	}
	void setup() {
		ofxDirList dir;
		int n;
		
		n = dir.listDir("animation/flocking");
		for(int i = 0; i < n; i++) {
			Animation* cur = new Animation();
			cur->setup(dir.getPath(i));
			flocking.push_back(cur);
		}
		
		n = dir.listDir("animation/attacking");
		for(int i = 0; i < n; i++) {
			Animation* cur = new Animation();
			cur->setup(dir.getPath(i));
			attacking.push_back(cur);
		}
	}
	~AnimationManager() {
		for(int i = 0; i < flocking.size(); i++) {
			delete flocking[i];
		}
		for(int i = 0; i < attacking.size(); i++) {
			delete attacking[i];
		}
	}
	Animation* randomFlocking() {
		return flocking[(int) ofRandom(0, flocking.size())];
	}
	Animation* randomAttacking() {
		return attacking[(int) ofRandom(0, attacking.size())];
	}
};