#include "Debris.h"

void Debris::setup(ofxVec2f position) {
	this->position = position;
	animation = AnimationManager::randomDebris();
}

void Debris::draw() {
	animation->draw(0);
}