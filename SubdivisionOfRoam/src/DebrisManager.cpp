#include "DebrisManager.h"

vector<Debris> DebrisManager::debris;

void DebrisManager::setup() {
}

void DebrisManager::add(ofPoint position) {
}

void DebrisManager::draw() {
	for(int i = 0; i < debris.size(); i++) {
		debris[i].draw();
	}
}
