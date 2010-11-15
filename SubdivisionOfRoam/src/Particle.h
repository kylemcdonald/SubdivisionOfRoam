#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "MSAPerlin.h"

#include "Animation.h"

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
	static float speed, spread, viscosity, independence, turbulence, neighborhood;
	
	static float animationBaseFramerate, animationScale, animationDepthScale, animationVelocityFramerate;
	static Animation animation;
	
	static vector<Particle> particles;
	static void setup();
  static void drawAll();
	static void drawAnimationAll();
	static void drawOrthoAll();
  static void updateAll();
	static void setSize(int size, float radius);

	float age;
  ofxVec3f position, velocity, force, localOffset;
	Particle() {
	}
  Particle(float radius) {
    randomize(localOffset);
  	randomize(position);
  	position *= radius;
		age = ofRandom(0, 10); // for frame offsets
  }
	inline void drawAnimation() {
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z * animationDepthScale);
		
		/*
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		float s = 100;
		glVertex3f(velocity.x * s, velocity.y * s, velocity.z * s);
		glEnd();
		*/
		
		float angle = atan2f(velocity.y, velocity.x);
		glRotatef(ofRadToDeg(angle), 0, 0, 1);
		if(angle > PI / 2 || angle < -PI / 2)
			glScalef(1, -1, 1);
		glScalef(animationScale, animationScale, 1);
		
		animation.draw(age);
		glPopMatrix();
	}
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
  }
  inline void applyCenteringForce() {
    centeringForce.set(position);
    centeringForce -= avg;
    float distanceToCenter = centeringForce.length();
    centeringForce.normalize();
    centeringForce *= -distanceToCenter / (spread * spread);
    force += centeringForce;
  }
  inline void update() {
    force.set(0, 0, 0);
    applyFlockingForce();
    applyViscosityForce();
		applyCenteringForce();
    velocity += force; // mass = 1
    position += velocity * speed;
		age += ofGetLastFrameTime() * animationBaseFramerate;
		age += velocity.length() * animationVelocityFramerate;
  }
};
