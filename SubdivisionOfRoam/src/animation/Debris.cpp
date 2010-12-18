#include "Debris.h"

float Debris::maxAge,
	Debris::scale;

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
	ofScale(scale, scale, scale);
	animation->drawPercent(getAge());
	glPopMatrix();
}

float Debris::getAge() {
	return (ofGetElapsedTimef() - birth) / maxAge;
}