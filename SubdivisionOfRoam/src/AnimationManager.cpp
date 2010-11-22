#include "AnimationManager.h"

vector<Animation*> AnimationManager::flocking, AnimationManager::attacking, AnimationManager::debris;

void AnimationManager::setup() {
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
	
	n = dir.listDir("animation/debris");
	for(int i = 0; i < n; i++) {
		Animation* cur = new Animation();
		cur->setup(dir.getPath(i));
		debris.push_back(cur);
	}
}

Animation* AnimationManager::randomFlocking() {
	return flocking[(int) ofRandom(0, flocking.size())];
}

Animation* AnimationManager::randomAttacking() {
	return attacking[(int) ofRandom(0, attacking.size())];
}

Animation* AnimationManager::randomDebris() {
	return debris[(int) ofRandom(0, debris.size())];
}