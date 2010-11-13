#include "testApp.h"

void testApp::setup(){
	vidPlayer.loadMovie("theo.mov");
	vidPlayer.play();
	
	colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);

	bLearnBakground = true;
	threshold = 80;
}

void testApp::update(){
	ofBackground(0);

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
	ofScale(2, 2, 1);
	
	ofSetColor(255);
	colorImg.draw(0, 0);
	/*
	grayImage.draw(320,0);
	grayBg.draw(0, 240);
	grayDiff.draw(320, 240);
	*/
	
	for (int i = 0; i < contourFinder.nBlobs; i++){
		ofxCvBlob& curBlob = contourFinder.blobs[i];
		
		/*ofSetColor(255);
		drawBlob(curBlob);
		drawNormals(curBlob, 4);*/
		
		ofSetColor(255);
		ofxCvBlob smoothed = curBlob;
		ContourMatcher::smoothBlob(smoothed, mouseY / 10, .5);
		drawBlob(smoothed);
		drawNormals(smoothed, 4);
		
		ofSetColor(255, 0, 255);
		ContourMatcher::resampleBlob(smoothed, mouseX + 1);
		drawBlob(smoothed);
		drawNormals(smoothed, 8);
		
	}
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
