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
	Particle::minimumSpeed,
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
	Particle::animationForceFramerate,
	Particle::animationVelocityFramerate,
	Particle::flapDisplacement;

float
	Particle::attackRange,
	Particle::attackPrecision,
	Particle::attackDetermination,
	Particle::attackAccuracy;

float
	Particle::groundForceStart,
	Particle::groundForceAmount,
	Particle::groundPosition;

EventTexture
	Particle::attackingTexture;

void Particle::setup() {	
	perlin.setup(4, 1, .5, (int) ofRandom(0, 1000));
	globalOffset.set(0, 1. / 3, 2. / 3);
	speed = 24;
  spread = 100;
  viscosity = .1;
  independence = .15;
  neighborhood = 700;
	turbulence = 1;

	attackingTexture.setup("sound/attacking");
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
			glColor4f(1, 0, 0, 1);
			glBegin(GL_LINE_STRIP);
			glVertex3fv(position.v);
			glVertex3fv(gaze.v);
			glVertex3fv(attackTarget.v);
			glEnd();
		} else {
			glColor4f(0, 0, 0, 1);
			glBegin(GL_LINES);
			glVertex3fv(position.v);
			glVertex3fv(gaze.v);
			glEnd();
		}
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
		
	float agePercent = (attackMode ? attackingAnimation : flockingAnimation)->getPercent(age);
	
	float angle = atan2f(velocity.y, velocity.x);
	glRotatef(ofRadToDeg(angle), 0, 0, 1);	
	if(angle > PI / 2 || angle < -PI / 2)
		glScalef(1, -1, 1);
	
	glTranslatef(0, cos(agePercent * TWO_PI + HALF_PI) * flapDisplacement, 0);
	glScalef(animationScale, animationScale, 1);
	
	glColor4f(1, 1, 1, 1);
	if(attackMode) {
		attackingAnimation->draw(age);
	} else {
		flockingAnimation->draw(age);
	}
	
	glPopMatrix();
	
	glPopMatrix();
}

void Particle::attackAtRandom() {
	vector<ofxCvBlob>& blobs = testApp::resampledBlobs;
	ofxCvBlob& curBlob = blobs[(int) ofRandom(0, blobs.size())];
	vector<ofPoint>& pts = curBlob.pts;
	ofPoint& curPoint = pts[(int) ofRandom(0, pts.size())];
	beginAttack(curPoint);
}

inline void Particle::update() {
	// calculate standard flocking stuff
	force.set(0, 0, 0);
	applyFlockingForce();
	applyViscosityForce();
	applyCenteringForce();
	applyAttackingForce();
	applyGroundAvoidanceForce();
	velocity += force; // mass = 1
	
	float curSpeed = velocity.length();
	velocity.normalize();
	velocity *= max(minimumSpeed, curSpeed);
	
	position += velocity * speed;
	
	age += ofGetLastFrameTime() * animationBaseFramerate;
	age += force.length() * animationForceFramerate;
	age += velocity.length() * animationVelocityFramerate;
	
	gaze = position + velocity * attackRange;
	
	checkForAttack();
	if(attackMode) {
		attackingTexture.update();
	}
}

inline void Particle::beginAttack(ofPoint& target) {
	// do one last check before attacking
	if(!HoleManager::intersects(target)) {
		attackMode = true;
		attackStarted = ofGetElapsedTimef();
		attackStartingPoint = position;
		attackTarget = target;
		
		SoundManager::randomSquawking();
	}
}

inline void Particle::endAttack() {
	attackMode = false;
	
	HoleManager::add(position);
	DebrisManager::add(position);
	SoundManager::randomRipping();
}

inline void Particle::checkForAttack() {
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
					beginAttack(pts[j]);
				}
			}
		}
	}
}