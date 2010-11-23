#include "Debris.h"

float Debris::maxAge;

void Debris::setup(ofxVec2f position) {
	birth = ofGetElapsedTimef();
	this->position = position;
	animation = AnimationManager::randomDebris();
	rotation = ofRandom(0, 360);
}

void Debris::draw() {
	glPushMatrix();
	ofTranslate(position);
	ofRotate(rotation);
	animation->drawPercent(getAge());
	glPopMatrix();
}

float Debris::getAge() {
	return (ofGetElapsedTimef() - birth) / maxAge;
}