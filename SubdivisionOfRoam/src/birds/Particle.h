#pragma once

#include "testApp.h"
#include "ofxVectorMath.h"
#include "MSAPerlin.h"

#include "AnimationManager.h"
#include "DebrisManager.h"

#include "EventTexture.h"

#include "Chunk.h"

inline void randomize(ofxVec3f& v) {
	v.x = ofRandomf();
	v.y = ofRandomf();
	v.z = ofRandomf();
	v.normalize();
}

class Particle {
public:
	static ofxVec3f centeringForce, globalOffset, avg;
	static MSA::Perlin perlin;
	static float minimumSpeed, speed, spread, viscosity, independence, turbulence, neighborhood;
	
	static float animationBaseFramerate, animationScale, animationDepthScale, animationForceFramerate, animationVelocityFramerate, flapDisplacement;
	
	static float attackRange, attackPrecision, attackDetermination, attackAccuracy;
	static float groundForceStart, groundForceAmount, groundPosition;
	static float gravity;
	static float escapeDistance;
	
	static vector<Particle> particles;
	static void setup();
  static void drawAll();
	static void drawAnimationAll();
	static void drawOrthoAll();
  static void updateAll();
	static void setSize(int size, float radius);
	
	static EventTexture attackingTexture;

	float age;
  ofxVec3f position, velocity, force, localOffset;
	ofxVec3f gaze, attackTarget, attackStartingPoint;
	Animation* flockingAnimation;
	Animation* attackingAnimation;
	bool attackMode;
	bool hasChunk;
	float attackStarted;
	Chunk chunk;
	
	Particle() {		
	}
  Particle(float radius) {
    randomize(localOffset);
  	randomize(position);
  	position *= radius;
		
		age = ofRandom(0, 10); // for frame offsets
		flockingAnimation = AnimationManager::randomFlocking();
		attackingAnimation = AnimationManager::randomAttacking();
		attackMode = false;
		hasChunk = false;
		
		chunk.setup();
  }
	void drawAnimation();
  inline void draw() {
  	glVertex3fv(position.v);
	}
	inline void drawOrtho() {
		glVertex2fv(position.v);
  }
  inline void applyFlockingForce() {
		float basex = position.x / neighborhood;
		float basey = position.y / neighborhood;
		float basez = position.z / neighborhood;
    force.x +=
      perlin.get(
        basex + globalOffset.x + localOffset.x * independence,
        basey,
        basez);
		force.y +=
      perlin.get(
        basex,
        basey + globalOffset.y  + localOffset.y * independence,
        basez);
		force.z +=
      perlin.get(
        basex,
        basey,
        basez + globalOffset.z + localOffset.z * independence);
  }
  inline void applyViscosityForce() {
    force += velocity * -viscosity;
		if(gravity != 0) {
			force.y += gravity / velocity.length();
		}
  }
  inline void applyCenteringForce() {
    centeringForce.set(position);
    centeringForce -= avg;
    float distanceToCenter = centeringForce.length();
    centeringForce.normalize();
    centeringForce *= -distanceToCenter / (spread * spread);
		if(hasChunk) {
			force -= centeringForce; // run away with chunks
		} else {
			force += centeringForce; // otherwise, keep together
		}
  }
	inline void applyAttackingForce() {
		if(attackMode) {
			ofxVec3f targetDirection = attackTarget - position;
			if(targetDirection.length() < attackAccuracy) {
				endAttack();
			} else {
				targetDirection.normalize();
				force += targetDirection * attackDetermination;
			}
		}
	}
	inline void applyGroundAvoidanceForce() {
		if(!attackMode) {
			force.y -= powf(ofMap(position.y, groundForceStart, groundPosition, 0, 1, true), 2) * groundForceAmount;
		}
	}
  void update();
	void checkForAttack();
	void attackAtRandom();
	void beginAttack(ofPoint& target);
	void endAttack();
	
	static void setAttackingDensity(float density) {
		attackingTexture.density = density / particles.size();
	}
};
