#include "Chunk.h"

#include "HoleManager.h"

float Chunk::chunkScale;
float Chunk::carryDistance;
float Chunk::globalRotation;

void Chunk::setup() {
	leftHole = HoleManager::randomHoleImage();
	rightHole = HoleManager::randomHoleImage();
	rotation = ofRandom(0, 360);
}

void Chunk::draw() {
	glPushMatrix();
	glTranslatef(0, leftHole->getHeight() * carryDistance, 0);
	ofRotateZ(rotation + globalRotation);
	glScalef(chunkScale, chunkScale, chunkScale);
	glTranslatef(0, -leftHole->getHeight() / 2, 0);
	
	//glColor4f(1, 0, 0, 1);
	//ofRect(0, 0, 50, 50);
	
	glColor4f(0, 0, 0, 1);
	rightHole->draw(0, 0);
	glScalef(-1, 1, 1);
	leftHole->draw(0, 0);
	glPopMatrix();
}