#include "AnimationManager.h"

Animation* AnimationManager::forward;
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
	
	forward = new Animation();
	forward->setup("animation/forward");
}

Animation* AnimationManager::randomFlocking() {
	return flocking[rand() % flocking.size()];
}

Animation* AnimationManager::randomAttacking() {
	return attacking[rand() % attacking.size()];
}

Animation* AnimationManager::randomDebris() {
	return debris[rand() % debris.size()];
}