#include "DebrisManager.h"

vector<Debris> DebrisManager::debris;

void DebrisManager::setup() {
}

void DebrisManager::add(ofPoint position) {
	debris.push_back(Debris());
	debris.back().setup(position);
	cout << debris.size() << endl;
}

void DebrisManager::update() {
	vector<Debris> remaining;
	for(int i = 0; i < debris.size(); i++) {
		if(debris[i].getAge() < 1) {
			remaining.push_back(debris[i]);
		}
	}
	debris = remaining;
}

void DebrisManager::draw() {	
	for(int i = 0; i < debris.size(); i++) {
		debris[i].draw();
	}
}