#include "Particle.h"

vector<Particle>
	Particle::particles;

ofxVec3f
	Particle::centeringForce,
	Particle::globalOffset,
	Particle::avg;

MSA::Perlin
	Particle::perlin;

float
	Particle::speed,
	Particle::spread,
	Particle::viscosity,
	Particle::independence,
	Particle::neighborhood,
	Particle::turbulence;

float
	Particle::animationBaseFramerate,
	Particle::animationScale,
	Particle::animationDepthScale,
	Particle::animationVelocityFramerate;
AnimationManager
	Particle::animationManager;

float
	Particle::attackRange,
	Particle::attackPrecision;

void Particle::setup() {
	perlin.setup(4, 1, .5, (int) ofRandom(0, 1000));
	globalOffset.set(0, 1. / 3, 2. / 3);
	speed = 24;
  spread = 100;
  viscosity = .1;
  independence = .15;
  neighborhood = 700;
	turbulence = 1;
	
	animationManager.setup();	
}

void Particle::drawAll() {
	glBegin(GL_POINTS);
	for(int i = 0; i < particles.size(); i++)
		particles[i].draw();
	glEnd();
}

void Particle::drawAnimationAll() {
	for(int i = 0; i < particles.size(); i++)
		particles[i].drawAnimation();
}

void Particle::drawOrthoAll() {
	glBegin(GL_POINTS);
	for(int i = 0; i < particles.size(); i++)
		particles[i].drawOrtho();
	glEnd();
}

void Particle::updateAll() {
	avg.set(0, 0, 0);
	ofxVec3f sum;
	for(int i = 0; i < particles.size(); i++) {
		particles[i].update();
		sum += particles[i].position;
	}
	avg = sum / particles.size();
	globalOffset += turbulence / neighborhood;
}

void Particle::setSize(int size, float radius) {
	while(particles.size() < size) {
		particles.push_back(Particle(radius));
	}
	if(particles.size() > size) {
		particles.resize(size);
	}
}

inline void Particle::drawAnimation() {
	glPushMatrix();
	glScalef(1, 1, animationDepthScale);
	
	if(testApp::debug) {
		glPushMatrix();
		if(attackMode) {
			glColor4f(1, 1, 0, 1);
			glBegin(GL_LINE_STRIP);
			glVertex3fv(position.v);
			glVertex3fv(gaze.v);
			glVertex3fv(target.v);
			glEnd();
		} else {
			glColor4f(.8, .4, 0, .5);
			glBegin(GL_LINES);
			glVertex3fv(position.v);
			glVertex3fv(gaze.v);
			glEnd();
		}
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	float angle = atan2f(velocity.y, velocity.x);
	glRotatef(ofRadToDeg(angle), 0, 0, 1);
	if(angle > PI / 2 || angle < -PI / 2)
		glScalef(1, -1, 1);
	glScalef(animationScale, animationScale, 1);
	
	glColor4f(1, 1, 1, 1);
	animation->draw(age);
	glPopMatrix();
	
	glPopMatrix();
}

inline void Particle::update() {
	force.set(0, 0, 0);
	applyFlockingForce();
	applyViscosityForce();
	applyCenteringForce();
	velocity += force; // mass = 1
	position += velocity * speed;
	
	age += ofGetLastFrameTime() * animationBaseFramerate;
	age += velocity.length() * animationVelocityFramerate;
	
	gaze = position + velocity * attackRange;
	
	attackMode = false;
	ofxCvBlob* targetBlob;
	int targetPoint;
	if(abs(gaze.z) < attackPrecision) { // ignore anything too far from z = 0
		vector<ofxCvBlob>& blobs = testApp::resampledBlobs;
		float bestDist = 0;
		for(int i = 0; i < blobs.size(); i++) {
			ofxCvBlob& cur = blobs[i];
			vector<ofPoint>& pts = cur.pts;
			for(int j = 0; j < pts.size(); j++) {
				float dist = gaze.distance(pts[j]);
				if(dist < attackPrecision && (!attackMode || dist < bestDist)) {
					bestDist = dist;
					attackMode = true;
					target = pts[j];
					targetPoint = j;
					targetBlob = &cur;
				}
			}
		}
	}
	
	// instead of just attacking, we need to lerp into an attack,
	// bring up the animation + sounds, and fade the motion
	if(attackMode == true) {
		// doesn't work with holes on the loop boundary
		int bit = 5;
		testApp::holes.push_back(Hole());
		// need to map targetPoint back onto targetBlob
		// how do we keep there from being too many holes?
		// holes need to disappear over time
		//testApp::holes.back().setup(*targetBlob, targetPoint - bit, targetPoint + bit);
	}
	
	// store time that the attack starts
	// use this for fading between attacking and flocking
}