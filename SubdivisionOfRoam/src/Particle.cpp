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
	glTranslatef(position.x, position.y, position.z * animationDepthScale);
	
	if(testApp::debug) {
		if(attackMode)
			glColor4f(1, 0, 0, 1);
		else
			glColor4f(0, 1, 1, 1);
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		ofxVec3f range = velocity * attackRange;
		glVertex3fv(range.v);
		glEnd();
	}
	
	float angle = atan2f(velocity.y, velocity.x);
	glRotatef(ofRadToDeg(angle), 0, 0, 1);
	if(angle > PI / 2 || angle < -PI / 2)
		glScalef(1, -1, 1);
	glScalef(animationScale, animationScale, 1);
	
	glColor4f(1, 1, 1, 1);
	animation->draw(age);
	glPopMatrix();
}