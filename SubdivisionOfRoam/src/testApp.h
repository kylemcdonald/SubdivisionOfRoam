#pragma once

#include "ofMain.h"

// core addons
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "ofxFbo.h"

// extra addons
#include "ofxControlPanel.h" // https://github.com/ofTheo/ofxControlPanel
#include "ofxLibdcPtGrey.h" // https://github.com/kylemcdonald/ofxLibdc

#include "ofxBlur.h"
#include "ofxAutoControlPanel.h"
#include "ContourUtils.h"
#include "Particle.h"

#include "AnimationManager.h"
#include "HoleManager.h"
#include "SoundManager.h"
#include "LoopTexture.h"
#include "EventTexture.h"

#include "Chunk.h"

class testApp : public ofBaseApp{
public:
	static bool debug;
	
	void setup();
	void setupControlPanel();
	void update();
	void draw();
	void drawWarped();

	void keyPressed(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void drawBlob(ofxCvBlob& blob);
	void drawNormals(ofxCvBlob& blob, float length);
	
	ofVideoGrabber grabber;
	ofxLibdcPtGrey camera;
	ofImage curFrame;
	
	ofxCvGrayscaleImage grayImage, grayDiff;
	ofxCvFloatImage grayBg;

	static ofxCvContourFinder contourFinder;
	static vector<ofxCvBlob> resampledBlobs;

	int threshold;
	bool bLearnBakground;
	
	ofxAutoControlPanel panel;
	
	ofxCvGrayscaleImage staticShadow;
	
	ofxFbo fbo;
	ofxBlur blur;
	
	LoopTexture ambienceTexture;
	EventTexture flappingTexture;
	
	static const int targetWidth = 1080;
	static const int targetHeight = 1920;
};
