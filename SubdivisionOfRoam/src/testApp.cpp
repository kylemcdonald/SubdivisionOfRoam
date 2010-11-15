#include "testApp.h"

void testApp::setup(){
	ofSetFrameRate(120);
	
	ofSetLogLevel(OF_LOG_VERBOSE);

	//vidPlayer.loadMovie("theo.mov");
	vidPlayer.play();
	
	colorImg.allocate(320, 240);
	grayImage.allocate(320, 240);
	grayBg.allocate(320, 240);
	grayDiff.allocate(320, 240);

	bLearnBakground = true;
	threshold = 80;
	
	Particle::setup();
	
	int n = 200;
	float radius = 250;
	for(int i = 0; i < n; i++)
		Particle::particles.push_back(Particle(radius));
	
	ofEnableAlphaBlending();
	
	panel.setup("Control Panel", 5, 5, 300, 600);
	panel.addPanel("animation");
	panel.addSlider("base framerate", "animationBaseFramerate", 20, 0, 30);
	panel.addSlider("velocity framerate", "animationVelocityFramerate", 0, 0, 1);
	panel.addSlider("scale", "animationScale", .15, 0, 2);
	panel.addSlider("depth scale", "animationDepthScale", 1, 0, 10);
	
	panel.addPanel("flocking");
	panel.addSlider("size", "flockingSize", n, 1, 1000);
	panel.addSlider("speed", "flockingSpeed", 1, 0, 10);
	panel.addSlider("turbulence", "flockingTurbulence", 60, 1, 100);
	panel.addSlider("spread", "flockingSpread", 40, 10, 100);
	panel.addSlider("viscosity", "flockingViscosity", .15, 0, 1);
	panel.addSlider("independence", "flockingIndependence", .35, 0, 1);
	panel.addSlider("neighborhood", "flockingNeighborhood", 200, 10, 1000);
	
	panel.addPanel("blob");
	panel.addSlider("smoothing size", "blob.smoothingSize", 0, 0, 10, true);
	panel.addSlider("smoothing amount", "blob.smoothingAmount", 0, 0, 1);
	panel.addSlider("resample number", "blob.resampleNumber", 2, 2, 1000, true);
}

void testApp::update() {
	Particle::animationBaseFramerate = panel.getValueF("animationBaseFramerate");
	Particle::animationVelocityFramerate = panel.getValueF("animationVelocityFramerate");
	Particle::animationScale = panel.getValueF("animationScale");
	Particle::animationDepthScale = panel.getValueF("animationDepthScale");
	
	Particle::setSize(panel.getValueI("flockingSize"), 250);
	Particle::speed = panel.getValueF("flockingSpeed") * ofGetLastFrameTime() * 256;
	Particle::spread = panel.getValueF("flockingSpread");
	Particle::viscosity = panel.getValueF("flockingViscosity");
	Particle::independence = panel.getValueF("flockingIndependence");
	Particle::neighborhood = panel.getValueF("flockingNeighborhood");
	Particle::turbulence = panel.getValueF("flockingTurbulence") * ofGetLastFrameTime();
	Particle::updateAll();

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
}


void testApp::draw(){
	ofBackground(255, 255, 255);
	
	glPushMatrix();
	
	glColor4f(0, 0, 0, 1);
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	Particle::drawAnimationAll();
	
	/*	
	for (int i = 0; i < contourFinder.nBlobs; i++){
		ofxCvBlob& curBlob = contourFinder.blobs[i];
		
		ofSetColor(255);
		drawBlob(curBlob);
		drawNormals(curBlob, 4);
		
		ofSetColor(0, 255, 0);
		ofxCvBlob smoothed = curBlob;
		ContourMatcher::smoothBlob(smoothed, panel.getValueI("smoothingSize"), panel.getValueF("smoothingAmount"));
		drawBlob(smoothed);
		drawNormals(smoothed, 4);
		
		ofSetColor(255, 0, 255);
		ContourMatcher::resampleBlob(smoothed, panel.getValueI("resampleNumber"));
		drawBlob(smoothed);
		drawNormals(smoothed, 8);
	}
	*/
	
	glPopMatrix();
	
}

void testApp::drawBlob(ofxCvBlob& blob) {
	vector<ofPoint>& pts = blob.pts;
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i < pts.size(); i++) {
		glVertex2f(pts[i].x, pts[i].y);
	}
	glEnd();
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
