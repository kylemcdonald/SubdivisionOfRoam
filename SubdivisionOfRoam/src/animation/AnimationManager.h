#pragma once

#include "Animation.h"

class AnimationManager {
	static vector<Animation*> flocking, attacking, debris;
public:
	static void setup();
	static Animation* randomFlocking();
	static Animation* randomAttacking();
	static Animation* randomDebris();
	
	static Animation* forward;
	static Animation* flipping;
};