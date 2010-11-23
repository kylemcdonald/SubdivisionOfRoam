#include "Hole.h"

int Hole::holeRadius;
float Hole::maxHoleAge;
bool Hole::useEllipses;
float Hole::deshake;

Hole::Hole() :
matchedBlob(NULL) {
	birth = ofGetElapsedTimef();
}

void Hole::setup(ofxVec2f position) {
	img = HoleManager::randomHoleImage();
	this->position = position;
}

void Hole::update() {
	vector<ofxCvBlob>& blobs = testApp::contourFinder.blobs;
	ofPoint closestPoint = ContourUtils::closestPoint(blobs, position, matchedBlob, matchedIndex);
	position.interpolate(closestPoint, deshake);
}

void Hole::draw() {
	if(matchedBlob != NULL) {
		if(useEllipses) {
			ofSetColor(255);
			ofEllipse(position.x, position.y, holeRadius, holeRadius);
		} else {
			int startIndex, stopIndex;
			ofxVec2f firstPoint, lastPoint;
			ContourUtils::getCenteredSegment(*matchedBlob, matchedIndex, holeRadius, startIndex, stopIndex, firstPoint, lastPoint);
			
			vector<ofPoint>& pts = matchedBlob->pts;
			
			if(testApp::debug) {
				ofSetColor(255, 0, 0);
				ofCircle(firstPoint, 5);
				ofSetColor(0, 0, 255);
				ofCircle(lastPoint, 5);
				ofSetColor(0, 255, 0);
				ofLine(firstPoint, lastPoint);
			}
			
			glColor3f(1, 1, 1);
			
			// first draw white polygon
			ofFill();
			ofBeginShape();
			ofVertex(firstPoint);
			
			if(startIndex == stopIndex) {
				// only need one vertex
				ofVertex(pts[startIndex]);
			} else if(startIndex < stopIndex) {
				// they're in order, one pass
				for(int i = startIndex; i <= stopIndex; i++) {
					ofVertex(pts[i]);
				}
			} else {
				// cross over the seam, do the first part
				for(int i = startIndex; i < pts.size(); i++) {
					ofVertex(pts[i]);
				}
				// then the second part
				for(int i = 0; i <= stopIndex; i++) {
					ofVertex(pts[i]);
				}					
			}
			
			ofVertex(lastPoint);
			ofEndShape();
			
			// then draw hole image on top of that
			glPushMatrix();
			glTranslatef(firstPoint.x, firstPoint.y, 0);
			ofxVec2f diff = lastPoint - firstPoint;
			float angle = atan2f(diff.y, diff.x) - PI / 2;
			glRotatef(ofRadToDeg(angle), 0, 0, 1);
			float scale = diff.length() / img->getHeight();
			glScalef(scale, scale, 1);
			img->draw(0, 0);
			ofNoFill();
			if(testApp::debug) {
				ofRect(0, 0, img->getWidth(), img->getHeight());
			}
			glPopMatrix();
		}
	}
}

float Hole::distance(ofPoint& target) {
	return position.distance(target);
}

float Hole::getAge() {
	return ofGetElapsedTimef() - birth;
}