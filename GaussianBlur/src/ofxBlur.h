#pragma once

#include "ofxShader.h"
#include "ofxFbo.h"

class ofxBlur {
private:
	ofxFbo ping, pong;
	ofxShader horizontalBlur, verticalBlur;
	int passes;
	float radius;
	
public:
	void setup(int width, int height) {
		ping.setup(width, height);
		pong.setup(width, height);
		horizontalBlur.setup("", "shaders/horizontalBlur.fs");
		verticalBlur.setup("", "shaders/verticalBlur.fs");
	}
	void setPasses(int passes) {
		this->passes = passes;
	}
	void setRadius(float radius) {
		this->radius = radius;
	}
	void begin() {
		ofPushStyle();
		ofPushMatrix();
		pong.begin();
	}
	void end() {
		pong.end();
		
		glColor4f(1, 1, 1, 1);
		for(int i = 0; i < passes; i++) {	
			ping.begin();
			horizontalBlur.begin();
			horizontalBlur.setUniform1f("radius", radius);
			pong.draw(0, 0);
			horizontalBlur.end();
			ping.end();
			
			pong.begin();
			verticalBlur.begin();
			verticalBlur.setUniform1f("radius", radius);
			ping.draw(0, 0);
			verticalBlur.end();
			pong.end();
		}
		
		pong.draw(0, 0);
		ofPopStyle();
		ofPopMatrix();
	}
};