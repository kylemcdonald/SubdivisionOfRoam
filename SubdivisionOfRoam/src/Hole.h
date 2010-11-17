#pragma once

#include "HoleManager.h"

class Hole {
protected:
	ofImage* img;
	ofxCvBlob* blob;
	int start, stop;
public:
	Hole() {
	}
	void setup() {
		img = HoleManager::randomHoleImage();
	}
	void attach(ofxCvBlob& blob, int start, int stop) {
		this->blob = &blob;
		this->start = start;
		this->stop = stop;
	}
	void draw() {
		if(start < stop) {
			glColor4f(1, 0, 0, 1);
			vector<ofPoint>& points = blob->pts;
			
			glColor3f(1, 1, 1);
			ofBeginShape();
			for(int i = start; i < stop; i++) {
				ofPoint& cur = points[i];
				ofVertex(cur.x, cur.y);
			}
			ofEndShape();
			
			ofxVec2f startPoint(points[start]);
			ofxVec2f stopPoint(points[stop - 1]);
			ofxVec2f diff = stopPoint - startPoint;
			float angle = atan2f(diff.y, diff.x) - PI / 2;
			glPushMatrix();
			glTranslatef(startPoint.x, startPoint.y, 0);
			glRotatef(ofRadToDeg(angle), 0, 0, 1);
			float scale = diff.length() / img->getHeight();
			glScalef(scale, scale, 1);
			img->draw(0, 0);			
			glPopMatrix();
		}
	}
};