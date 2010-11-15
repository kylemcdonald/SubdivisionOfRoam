#pragma once

#include "ofxControlPanel.h"
#include "ofEvents.h"

class ofxAutoControlPanel : public ofxControlPanel {
public:
	ofxAutoControlPanel() {
		ofAddListener(ofEvents.update, this, &ofxAutoControlPanel::update);
		ofAddListener(ofEvents.draw, this, &ofxAutoControlPanel::draw);
		ofAddListener(ofEvents.keyPressed, this, &ofxAutoControlPanel::keyPressed);
		ofAddListener(ofEvents.mousePressed, this, &ofxAutoControlPanel::mousePressed);
		ofAddListener(ofEvents.mouseReleased, this, &ofxAutoControlPanel::mouseReleased);
		ofAddListener(ofEvents.mouseDragged, this, &ofxAutoControlPanel::mouseDragged);
	}
	void update(ofEventArgs& event) {
		ofxControlPanel::update();
	}
	void draw(ofEventArgs& event) {
		if(!hidden)
			ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 40, ofGetHeight() - 10);
		ofxControlPanel::draw();
	}
	void keyPressed(ofKeyEventArgs& event) {
		if(event.key == '\t') {
			if(hidden) {
				show();
			} else {
				hide();
			}
		}
	}
	void mousePressed(ofMouseEventArgs& event) {
		ofxControlPanel::mousePressed(event.x, event.y, event.button);
	}
	void mouseReleased(ofMouseEventArgs& event) {
		ofxControlPanel::mouseReleased();
	}
	void mouseDragged(ofMouseEventArgs& event) {
		ofxControlPanel::mouseDragged(event.x, event.y, event.button);
	}
	
	// by default, make sliders float
	void addSlider(string name, string xmlName, float value, float low, float high, bool isInt = false) {
		ofxControlPanel::addSlider(name, xmlName, value, low, high, isInt);
	}
	void addPanel(string panelName, int columns = 1) {
		ofxControlPanel::addPanel(panelName, columns);
		ofxControlPanel::setWhichPanel(panelName);
	}
};
