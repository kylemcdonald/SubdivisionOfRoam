#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "ofxControlPanel.h"
#include "ofxFbo.h"

#include "ContourMatcher.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void drawBlob(ofxCvBlob& blob);
	void drawNormals(ofxCvBlob& blob, float length);

	ofVideoPlayer vidPlayer;
	
	ofxCvColorImage colorImg;

	ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage grayBg;
	ofxCvGrayscaleImage grayDiff;

	ofxCvContourFinder contourFinder;

	int threshold;
	bool bLearnBakground;
};
