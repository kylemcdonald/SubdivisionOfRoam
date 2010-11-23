#include "Chunk.h"

#include "HoleManager.h"

float Chunk::chunkScale;
float Chunk::carryDistance;

void Chunk::setup() {
	leftHole = HoleManager::randomHoleImage();
	rightHole = HoleManager::randomHoleImage();
	rotation = ofRandom(0, 360);
}

void Chunk::draw() {
	glColor4f(0, 0, 0, 1);
	glPushMatrix();
	glTranslatef(0, leftHole->getHeight() * carryDistance, 0);
	ofRotateZ(rotation);
	glTranslatef(0, -leftHole->getHeight() / 2, 0);
	glScalef(chunkScale, chunkScale, chunkScale);
	leftHole->draw(0, 0);
	glScalef(-1, 1, 1);
	rightHole->draw(0, 0);
	glPopMatrix();
}