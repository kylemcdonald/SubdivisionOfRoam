#include "testApp.h"

// have birds attack
// birds remove chunks
// make it work with video
// first pass of contour tracking as just using the same point number
// add sounds
// add feather explosions
// render through 1920x1080 FBO + quad warping
// threshold incoming video for proper camera-based interaction
// birds should have momentum and no awkward moments
// shouldn't bite things touching the bottom of the screen

// debug visualization:
//	birds trajectories
//	rect outlines
//	blob detection and normals
//	attack state

bool testApp::debug = false;

void testApp::setup(){
	ofSetFrameRate(120);
	
	camera.setPosition((752 - 640) / 2, 0);
	camera.setFormat7(true);
	camera.setup();
	camera.setMaxFramerate();
	
	ofImage img;
	img.loadImage("people.png");
	img.setImageType(OF_IMAGE_GRAYSCALE);
	staticShadow.allocate(640, 480);
	memcpy(staticShadow.getPixels(), img.getPixels(), 640 * 480);
	staticShadow.invert();
	staticShadow.flagImageChanged();
	
	HoleManager::setup();
	
	ofSetLogLevel(OF_LOG_VERBOSE);

	//vidPlayer.loadMovie("theo.mov");
	//vidPlayer.play();
	
	colorImg.allocate(320, 240);
	grayImage.allocate(320, 240);
	grayBg.allocate(320, 240);
	grayDiff.allocate(320, 240);

	bLearnBakground = true;
	threshold = 80;
	
	Particle::setup();
	
	int n = 500;
	float radius = 250;
	for(int i = 0; i < n; i++)
		Particle::particles.push_back(Particle(radius));
	
	ofEnableAlphaBlending();
	
	panel.setXMLFilename("roamSettings.xml");
	panel.setup("Control Panel", 5, 5, 300, 600);
	panel.addPanel("general");
	panel.addToggle("debug", "debug", true);
	
	panel.addPanel("animation");
	panel.addSlider("base framerate", "animationBaseFramerate", 5, 0, 60);
	panel.addSlider("velocity framerate", "animationVelocityFramerate", .5, 0, 4);
	panel.addSlider("scale", "animationScale", .15, 0, 2);
	panel.addSlider("depth scale", "animationDepthScale", 2, 0, 10);
	
	panel.addPanel("flocking");
	panel.addToggle("enable", "flockingEnable", true);
	panel.addSlider("size", "flockingSize", n, 1, 1000);
	panel.addSlider("speed", "flockingSpeed", 1, 0, 10);
	panel.addSlider("turbulence", "flockingTurbulence", 60, 1, 100);
	panel.addSlider("spread", "flockingSpread", 60, 10, 100);
	panel.addSlider("viscosity", "flockingViscosity", .15, 0, 1);
	panel.addSlider("independence", "flockingIndependence", .35, 0, 1);
	panel.addSlider("neighborhood", "flockingNeighborhood", 200, 10, 1000);
	
	panel.addPanel("attacking");
	panel.addSlider("range", "attackingRange", 200, 10, 600);
	panel.addSlider("precision", "attackingPrecision", 20, 1, 50);
	
	panel.addPanel("blob");
	panel.addSlider("smoothing size", "blobSmoothingSize", 0, 0, 10, true);
	panel.addSlider("smoothing amount", "blobSmoothingAmount", 0, 0, 1);
	panel.addSlider("resample number", "blobResampleNumber", 100, 2, 500, true);
	
	panel.addDrawableRect("input", &curFrame, 200, 150);
	
	panel.addPanel("holes");
	panel.addToggle("randomize", "holeRandomize", false);
	panel.addSlider("random seed", "holeRandomSeed", 0, 0, 1000, true);
	panel.addSlider("count", "holeCount", 50, 0, 200);
	panel.addSlider("max size", "holeMaxSize", 20, 0, 80);
}

void testApp::update() {
	debug = panel.getValueB("debug");
	
	Particle::animationBaseFramerate = panel.getValueF("animationBaseFramerate");
	Particle::animationVelocityFramerate = panel.getValueF("animationVelocityFramerate");
	Particle::animationScale = panel.getValueF("animationScale");
	Particle::animationDepthScale = panel.getValueF("animationDepthScale");
	Particle::attackRange = panel.getValueF("attackingRange");
	Particle::attackPrecision = panel.getValueF("attackingPrecision");
	
	if(panel.getValueB("flockingEnable")) {
		Particle::setSize(panel.getValueI("flockingSize"), 250);
		Particle::speed = panel.getValueF("flockingSpeed") * ofGetLastFrameTime() * 256;
		Particle::spread = panel.getValueF("flockingSpread");
		Particle::viscosity = panel.getValueF("flockingViscosity");
		Particle::independence = panel.getValueF("flockingIndependence");
		Particle::neighborhood = panel.getValueF("flockingNeighborhood");
		Particle::turbulence = panel.getValueF("flockingTurbulence") * ofGetLastFrameTime();
		Particle::updateAll();
	}
	
	if(camera.grabVideo(curFrame)) {
		curFrame.update();
	}
	
	/*
	bool bNewFrame = false;

	vidPlayer.idleMovie();
	bNewFrame = vidPlayer.isFrameNew();
	
	if (bNewFrame){

		colorImg.setFromPixels(vidPlayer.getPixels(), 320, 240);
	
		grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;
			bLearnBakground = false;
		}

		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		contourFinder.findContours(grayDiff, 20, (340*240) / 3, 10, false, true);
	}
	*/
	
	contourFinder.findContours(staticShadow, 50, 640 * 480 * .2, 8, true, true);
	
	ofPoint offset(-640 / 2, (ofGetHeight() / 2) - 480);
	holes.clear();
	for(int i = 0; i < contourFinder.nBlobs; i++) {
		ofxCvBlob& curBlob = contourFinder.blobs[i];
		vector<ofPoint>& pts = curBlob.pts;
		for(int j = 0; j < pts.size(); j++) {
			pts[j] += offset;
		}
		
		if(panel.getValueB("holeRandomize")) {
			ofSeedRandom(panel.getValueI("holeRandomSeed"));
			for(int i = 0; i < panel.getValueI("holeCount"); i++) {
				int start = ofRandom(0, pts.size());
				int stop = ofRandom(start, min((int) pts.size(), start + panel.getValueI("holeMaxSize")));
				holes.push_back(Hole());
				holes.back().setup();
				holes.back().attach(curBlob, start, stop);
			}
		}
	}
}


void testApp::draw(){
	glDisable(GL_DEPTH_TEST);
	
	ofBackground(255, 255, 255);
	
	glPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	
	if(debug) {
		ofRotateY(60 * sin(ofMap(mouseX, 0, ofGetWidth(), -HALF_PI, +HALF_PI)));
	}
	
	for (int i = 0; i < contourFinder.nBlobs; i++){
		ofxCvBlob& curBlob = contourFinder.blobs[i];
		ofSetColor(0);
		ofFill();
		drawBlob(curBlob);
	}
	
	for (int i = 0; i < holes.size(); i++){
		holes[i].draw();
	}
	
	if(debug) {
		for (int i = 0; i < contourFinder.nBlobs; i++){
			ofxCvBlob& curBlob = contourFinder.blobs[i];
		
			drawNormals(curBlob, 4);
			
			ofSetColor(0, 255, 0);
			ofNoFill();
			ofxCvBlob smoothed = curBlob;
			ContourMatcher::smoothBlob(smoothed, panel.getValueI("blobSmoothingSize"), panel.getValueF("blobSmoothingAmount"));
			drawBlob(smoothed);
			drawNormals(smoothed, 4);
			
			ofSetColor(255, 0, 255);
			ContourMatcher::resampleBlob(smoothed, panel.getValueI("blobResampleNumber"));
			drawBlob(smoothed);
			drawNormals(smoothed, 8);
		}
	}
	
	Particle::drawAnimationAll();
	
	glPopMatrix();
}

void testApp::drawBlob(ofxCvBlob& blob) {
	vector<ofPoint>& pts = blob.pts;
	if(ofGetFill() == OF_FILLED) {
		ofBeginShape();
		for(int i = 0; i < pts.size(); i++) {
			ofVertex(pts[i].x, pts[i].y);
		}
		ofEndShape();
	} else {
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < pts.size(); i++) {
			glVertex2f(pts[i].x, pts[i].y);
		}
		glEnd();
	}
}

void testApp::drawNormals(ofxCvBlob& blob, float length) {
	vector<ofPoint>& pts = blob.pts;
	for(int i = 1; i < pts.size(); i++) {
		ofPoint& prev = pts[i - 1];
		ofPoint& cur = pts[i];
		ofLine(prev.x, prev.y, cur.x, cur.y);
		ofxVec2f diff = cur - prev;
		diff.rotate(90);
		diff.normalize();
		diff *= length;
		diff += cur;
		ofLine(cur.x, cur.y, diff.x, diff.y);
	}
}

void testApp::keyPressed(int key){
	switch (key){
		case 'd':
			panel.setValueB("debug", panel.getValueB("debug") ? false : true);
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case '[':
			vidPlayer.setPaused(true);
			break;
		case ']':
			vidPlayer.setPaused(false);
			break;
	}
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mouseDragged(int x, int y, int button){
}

void testApp::mousePressed(int x, int y, int button){
}

void testApp::mouseReleased(int x, int y, int button){
}
