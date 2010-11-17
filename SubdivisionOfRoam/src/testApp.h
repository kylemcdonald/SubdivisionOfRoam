#pragma once

#include "ofMain.h"

// core addons
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "ofxFbo.h"

// extra addons
#include "ofxControlPanel.h" // https://github.com/ofTheo/ofxControlPanel
#include "ofxLibdcPtGrey.h" // https://github.com/kylemcdonald/ofxLibdc

#include "ofxAutoControlPanel.h"
#include "ContourMatcher.h"
#include "Particle.h"
#include "Hole.h"

class testApp : public ofBaseApp{
public:
	static bool debug;
	
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

	ofxLibdcPtGrey camera;
	ofImage curFrame;
	
	ofVideoPlayer vidPlayer;
	
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage grayBg;
	ofxCvGrayscaleImage grayDiff;

	ofxCvContourFinder contourFinder;

	int threshold;
	bool bLearnBakground;
	
	ofxAutoControlPanel panel;
	
	ofxCvGrayscaleImage staticShadow;
	vector<Hole> holes;
};
