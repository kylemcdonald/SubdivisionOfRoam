#pragma once

#include "ofMain.h"
#include "ofxVec3f.h"
#include "MSAPerlin.h"

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
	static vector<Particle> particles;
	static void setup();
  static void drawAll();
	static void drawOrthoAll();
  static void updateAll();
	static void setSize(int size, float radius);

  ofxVec3f position, velocity, force, localOffset;
	Particle() {
	}
  Particle(float radius) {
    randomize(localOffset);
  	randomize(position);
  	position *= radius;
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
  }
};
