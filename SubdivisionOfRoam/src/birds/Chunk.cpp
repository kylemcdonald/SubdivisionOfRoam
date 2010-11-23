#include "Chunk.h"

#include "HoleManager.h"

void Chunk::setup() {
	leftHole = HoleManager::randomHoleImage();
	rightHole = HoleManager::randomHoleImage();
}

void Chunk::draw() {
	glColor4f(0, 0, 0, 1);
	glPushMatrix();
	leftHole->draw(0, 0);
	glScalef(-1, 1, 1);
	rightHole->draw(0, 0);
	glPopMatrix();
}