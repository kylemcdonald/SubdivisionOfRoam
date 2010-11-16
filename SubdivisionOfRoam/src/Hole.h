#pragma once

class Hole {
protected:
	ofImage img;
public:
	Hole() {
	}
	void setup(string filename) {
		img.loadImage(filename);
	}
	void draw(ofxCvBlob& blob, int start, int stop) {
		if(start < stop) {
			glColor4f(1, 0, 0, 1);
			vector<ofPoint>& points = blob.pts;
			
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
			float scale = diff.length() / img.getHeight();
			glScalef(scale, scale, 1);
			img.draw(0, 0);
			glPopMatrix();
		}
	}
};