#include "testApp.h"

/*
 to finish:
 
 manual camera controls to reset camera on restart
  
 to perfect:
 
 allow birds to spread vertically more than horizontally
 animate neighborhood and independence values
 motion blur
 noise in animation playback
 shouldn't bite things touching the bottom of the screen
 flap primarily when moving upwards, not downwards
 distort birds as they fly with warping the textures, makes them sketchier
*/
 
bool testApp::debug = false;
ofxCvContourFinder testApp::contourFinder;
vector<ofxCvBlob> testApp::resampledBlobs;

void testApp::setup(){	
	ofSetLogLevel(OF_LOG_VERBOSE);
	glDisable(GL_DEPTH_TEST);
	
	blur.setup(targetWidth, targetHeight);
	
	ambienceTexture.setup("sound/ambience");
	flappingTexture.setup("sound/flapping");
	
	curFrame.allocate(640, 480, OF_IMAGE_GRAYSCALE);
	
	camera.setPosition((752 - 640) / 2, 0);
	camera.setFormat7(true);
	camera.setup();
	camera.setMaxFramerate();
	
	if(!camera.ready()) {
		grabber.initGrabber(640, 480);
	}
	
	ofImage img;
	img.loadImage("people.png");
	img.setImageType(OF_IMAGE_GRAYSCALE);
	staticShadow.allocate(640, 480);
	memcpy(staticShadow.getPixels(), img.getPixels(), 640 * 480);
	staticShadow.invert();
	staticShadow.flagImageChanged();
	
	fbo.setup(targetWidth, targetHeight);
	
	SoundManager::setup();
	HoleManager::setup();
	AnimationManager::setup();
	DebrisManager::setup();
	
	grayImage.allocate(camera.getWidth(), camera.getHeight());
	grayBg.allocate(camera.getWidth(), camera.getHeight());
	grayDiff.allocate(camera.getWidth(), camera.getHeight());
	bLearnBakground = true;
	
	Particle::setup();
	
	setupControlPanel();
}

void testApp::setupControlPanel() {	
	panel.setup("Control Panel", 540, 0, 300, 960);
	panel.addPanel("general");
	panel.addToggle("fullscreen", "fullscreen", false);
	panel.addToggle("debug", "debug", false);
	panel.addToggle("move camera", "moveCamera", false);
	panel.addToggle("use live video", "blobUseLiveVideo", false);
	panel.addToggle("reset background", "blobResetBackground", true);
	panel.addToggle("adapt background", "blobAdaptBackground", true);
	panel.addSlider("adapt rate", "blobAdaptRate", .0001, 0, .001);
	panel.addSlider("threshold", "blobThreshold", 128, 0, 255, true);
	panel.addDrawableRect("curFrame", &curFrame, 200, 150);
	panel.addDrawableRect("background", &grayBg, 200, 150);
	panel.addDrawableRect("difference", &grayDiff, 200, 150);

	panel.addPanel("input");
	panel.addSlider("brightness", "cameraBrightness", 0, 0, 1);
	panel.addSlider("exposure", "cameraExposure", 0, 0, 1);
	panel.addSlider("gain", "cameraGain", 0, 0, 1);
	panel.addSlider("shutter", "cameraShutter", 0, 0, 1);
	panel.addDrawableRect("curFrame", &curFrame, 200, 150);
	
	panel.addPanel("sound");
	panel.addToggle("enable", "soundEnable", true);
	panel.addSlider("ambience rate", "soundAmbienceRate", 2, 0, 5);
	panel.addSlider("ambience volume", "soundAmbienceVolume", .2, 0, 1);
	panel.addSlider("flapping volume", "soundFlappingVolume", .4, 0, 1);
	panel.addSlider("flapping density", "soundFlappingDensity", .6, 0, 1);
	panel.addSlider("ripping volume", "soundRippingVolume", .12, 0, 1);
	panel.addSlider("squawking volume", "soundSquawkingVolume", .1, 0, 1);
	panel.addSlider("attacking volume", "soundAttackingVolume", .25, 0, 1);
	panel.addSlider("attacking density", "soundAttackingDensity", 3, 0, 10);
	
	panel.addPanel("blur");
	panel.addSlider("global radius", "blurGlobalRadius", 1.8, 0, 20);
	panel.addSlider("global passes", "blurGlobalPasses", 1, 0, 20, true);
	
	panel.addPanel("animation");
	panel.addSlider("base framerate", "animationBaseFramerate", 1, 0, 40);
	panel.addSlider("force framerate", "animationForceFramerate", 5, 0, 40);
	panel.addSlider("velocity framerate", "animationVelocityFramerate", .2, 0, 4);
	panel.addSlider("flap displacement", "animationFlapDisplacement", 8, 0, 30);
	panel.addSlider("scale", "animationScale", .3, 0, 2);
	panel.addSlider("attack scale", "attackScale", .3, 0, 2);
	panel.addSlider("depth scale", "animationDepthScale", 2, 0, 10);
	panel.addSlider("debris max age", "animationDebrisMaxAge", .4, 0, 2);
	panel.addSlider("debris scale", "animationDebrisScale", .5, 0, 1);
	panel.addToggle("use forward", "animationUseForward", false);
	panel.addToggle("use flipping", "animationUseFlipping", true);
	panel.addSlider("forward angle", "animationForwardAngle", 0, 0, 90);
	
	panel.addPanel("flocking");
	panel.addToggle("enable", "flockingEnable", true);
	panel.addSlider("size", "flockingSize", 500, 1, 1000);
	panel.addSlider("minimum speed", "flockingMinimumSpeed", 1, 0, 4);
	panel.addSlider("speed", "flockingSpeed", 1.5, 0, 10);
	panel.addSlider("turbulence", "flockingTurbulence", 60, 1, 100);
	panel.addSlider("spread", "flockingSpread", 85, 10, 120);
	panel.addSlider("viscosity", "flockingViscosity", .15, 0, 1);
	panel.addSlider("independence", "flockingIndependence", .15, 0, 1);
	panel.addSlider("neighborhood", "flockingNeighborhood", 400, 10, 1000);
	panel.addSlider("gravity", "flockingGravity", 0, 0, 1);
	panel.addSlider("start position", "flockingStartPosition", targetHeight / 2, 0, targetHeight);
	
	panel.addPanel("attacking");
	panel.addSlider("ground force start", "groundForceStart", -960, -targetHeight, 0);
	panel.addSlider("ground force amount", "groundForceAmount", .44, 0, 5);
	panel.addSlider("ground position", "groundPosition", 725, 0, targetHeight);
	panel.addSlider("range", "attackingRange", 800, 10, targetHeight);
	panel.addSlider("precision", "attackingPrecision", 100, 1, 800);
	panel.addSlider("determination", "attackingDetermination", .4, 0, 1);
	panel.addSlider("accuracy", "attackingAccuracy", 40, 1, 80);
	panel.addSlider("chunk scale", "chunkScale", 1.56, 0, 5);
	panel.addSlider("chunk carry distance", "chunkCarryDistance", .52, 0, 1.5);
	panel.addSlider("escape distance", "escapeDistance", 2, 1, 6);
	
	panel.addPanel("blob");
	panel.addSlider("general motion", "blobGeneralMotion", 5, 0, 20);
	panel.addSlider("contour motion", "blobContourMotion", 2, 0, 10);
	panel.addSlider("min blob diameter", "blobMinDiameter", 20, 10, 100);
	panel.addSlider("max blob diameter", "blobMaxDiameter", 400, 10, 640);
	panel.addSlider("sample rate", "blobSampleRate", 8, 1, 16, true);
	panel.addSlider("smoothing size", "blobSmoothingSize", 3, 0, 10, true);
	panel.addSlider("smoothing amount", "blobSmoothingAmount", .4, 0, 1);
	panel.addSlider("resample spacing", "blobResampleSpacing", 10, 1, 10);
	
	panel.addPanel("holes");
	panel.addSlider("radius", "holeRadius", 19, 1, 40);
	panel.addSlider("spacing", "holeSpacing", 92, 1, 100);
	panel.addSlider("max age", "holeMaxAge", 30, 1, 60);
	panel.addToggle("use ellipses", "holeUseEllipses", false);
	panel.addSlider("deshake", "holeDeshake", 0.9, 0, 1);
	
	panel.addPanel("warp");
	panel.addToggle("flip orientation", "flipOrientation", false);
	
	float maxXPosition = 540. / (540 + 300);
	panel.addSlider("nwx", "warpNwx", 0, 0, 1);
	panel.addSlider("nwy", "warpNwy", 0, 0, 1);
	panel.addSlider("nex", "warpNex", maxXPosition, 0, 1);
	panel.addSlider("ney", "warpNey", 0, 0, 1);
	panel.addSlider("swx", "warpSwx", 0, 0, 1);
	panel.addSlider("swy", "warpSwy", 1, 0, 1);
	panel.addSlider("sex", "warpSex", maxXPosition, 0, 1);
	panel.addSlider("sey", "warpSey", 1, 0, 1);
	
	float fineRange = .01;
	panel.addSlider("nwx fine", "warpNwxFine", 0, -fineRange, fineRange);
	panel.addSlider("nwy fine", "warpNwyFine", 0, -fineRange, fineRange);
	panel.addSlider("nex fine", "warpNexFine", 0, -fineRange, fineRange);
	panel.addSlider("ney fine", "warpNeyFine", 0, -fineRange, fineRange);
	panel.addSlider("swx fine", "warpSwxFine", 0, -fineRange, fineRange);
	panel.addSlider("swy fine", "warpSwyFine", 0, -fineRange, fineRange);
	panel.addSlider("sex fine", "warpSexFine", 0, -fineRange, fineRange);
	panel.addSlider("sey fine", "warpSeyFine", 0, -fineRange, fineRange);
	
	panel.setXMLFilename("roamSettings.xml");
	panel.loadSettings("roamSettings.xml");
	panel.hide();
	
	panel.setValueB("blobResetBackground", true);
}

void testApp::update() {		
	if(panel.hasValueChanged("fullscreen")) {
		ofSetFullscreen(panel.getValueB("fullscreen"));
	}
	
	debug = panel.getValueB("debug");
	
	ambienceTexture.rate = panel.getValueF("soundAmbienceRate");
	ambienceTexture.overallVolume = panel.getValueF("soundAmbienceVolume");
	
	flappingTexture.density = panel.getValueF("soundFlappingDensity");
	Particle::setAttackingDensity(panel.getValueF("soundAttackingDensity"));
	
	if(panel.hasValueChanged("soundFlappingVolume")) {
		flappingTexture.setVolume(panel.getValueF("soundFlappingVolume"));
	}
	if(panel.hasValueChanged("soundRippingVolume")) {
		SoundManager::setRippingVolume(panel.getValueF("soundRippingVolume"));
	}
	if(panel.hasValueChanged("soundSquawkingVolume")) {
		SoundManager::setSquawkingVolume(panel.getValueF("soundSquawkingVolume"));
	}
	if(panel.hasValueChanged("soundAttackingVolume")) {
		Particle::attackingTexture.setVolume(panel.getValueF("soundAttackingVolume"));
	}
	
	SoundManager::enabled = panel.getValueB("soundEnable");
	ambienceTexture.update();
	flappingTexture.update();
	
	Hole::holeRadius = panel.getValueF("holeRadius");
	Hole::maxHoleAge = panel.getValueF("holeMaxAge");
	Hole::useEllipses = panel.getValueB("holeUseEllipses");
	Hole::deshake = panel.getValueF("holeDeshake");
	HoleManager::holeSpacing = panel.getValueF("holeSpacing");
	
	Debris::maxAge = panel.getValueF("animationDebrisMaxAge");
	Debris::scale = panel.getValueF("animationDebrisScale");
	
	if(panel.getValueB("flipOrientation")) {
		panel.setValueF("warpNwx", 1);
		panel.setValueF("warpNwy", 0);
		panel.setValueF("warpNex", 1);
		panel.setValueF("warpNey", 1);
		panel.setValueF("warpSex", 0);
		panel.setValueF("warpSey", 1);
		panel.setValueF("warpSwx", 0);
		panel.setValueF("warpSwy", 0);
		panel.setValueB("flipOrientation", false);
	}
	
	bool useLiveVideo = panel.getValueB("blobUseLiveVideo");
	bool cameraAvailable = camera.ready();
	if(useLiveVideo && cameraAvailable) {
		camera.setBrightnessNorm(panel.getValueF("cameraBrightness"));
		camera.setExposureNorm(panel.getValueF("cameraExposure"));
		camera.setGainNorm(panel.getValueF("cameraGain"));
		camera.setShutterNorm(panel.getValueF("cameraShutter"));
	}
	
	if(panel.getValueB("blobUseLiveVideo")) {
		bool updateFromCurFrame = false;
		if(camera.ready()) {
			if(camera.grabVideo(curFrame)) {
				updateFromCurFrame = true;
			}
		} else {
			grabber.grabFrame();
			if(grabber.isFrameNew()) {
				// pretty sure there aren't any ofConvertInto functions for color->grayscale conversion
				unsigned char* grabberPixels = grabber.getPixels();
				unsigned char* curPixels = curFrame.getPixels();
				int n = 640 * 480;
				int j = 0;
				for(int i = 0; i < n; i++) {
					unsigned int sum = grabberPixels[j++];
					sum += grabberPixels[j++];
					sum += grabberPixels[j++];
					curPixels[i] = sum / 3;
				}
				updateFromCurFrame = true;
			}
		}
		if(updateFromCurFrame) {
			curFrame.update();
			
			memcpy(grayImage.getPixels(), curFrame.getPixels(), curFrame.getWidth() * curFrame.getHeight());
			
			if(panel.getValueB("blobResetBackground")){
				grayBg = grayImage;
				grayBg.flagImageChanged();
				panel.setValueB("blobResetBackground", false);
			}
			
			if(panel.getValueB("blobAdaptBackground")) {
				grayBg.addWeighted(grayImage, panel.getValueF("blobAdaptRate"));
			}
			
			// grayDiff.absDiff(grayBg, grayImage);
			// replaced with float-compatible single ended diff
			int n = 640 * 480;
			unsigned char* diffPixels = grayDiff.getPixels();
			unsigned char* grayPixels = grayImage.getPixels();
			float* bgPixels = grayBg.getPixelsAsFloats();
			for(int i = 0; i < n; i++) {
				float cur = (bgPixels[i] * 255) - grayPixels[i];
				diffPixels[i] = cur < 0 ? 0 : cur;
			}
			
			grayDiff.threshold(panel.getValueI("blobThreshold"));
			grayDiff.flagImageChanged();
		}
	}
	
	// update blobs
	float minBlobSize = PI * powf(panel.getValueF("blobMinDiameter") / 2, 2);
	float maxBlobSize = PI * powf(panel.getValueF("blobMaxDiameter") / 2, 2);
	contourFinder.findContours(panel.getValueB("blobUseLiveVideo") ? grayDiff : staticShadow, minBlobSize, maxBlobSize, 16, true, true);
	float scaleBlobs = targetWidth / camera.getWidth();
	// this doesn't project the smallest part on the bottom but avoids showing people with their feet missing
	ofPoint offset(-camera.getWidth() / 2, targetHeight / 2 - (camera.getHeight() * scaleBlobs));
	float contourNoise = panel.getValueF("blobContourMotion");
	if(panel.getValueB("blobUseLiveVideo")) {
		contourNoise = 0;
	} else {
		offset += ofPoint(ofSignedNoise(ofGetElapsedTimef(), 1), ofSignedNoise(1, ofGetElapsedTimef())) * panel.getValueF("blobGeneralMotion");
	}
	for(int i = 0; i < contourFinder.nBlobs; i++) {
		// offset blobs
		ofxCvBlob& curBlob = contourFinder.blobs[i];
		vector<ofPoint>& pts = curBlob.pts;
		for(int j = 0; j < pts.size(); j++) {
			// could add something here to extend legs further instead of raising image
			pts[j] += offset;
			pts[j] *= scaleBlobs;
			if(contourNoise > 0) {
				pts[j].x += ofRandomf() * contourNoise;
				pts[j].y += ofRandomf() * contourNoise;
			}
		}
	}
	
	ContourUtils::smoothBlobs(contourFinder.blobs, panel.getValueI("blobSmoothingSize"), panel.getValueF("blobSmoothingAmount"));
	
	// resample blobs
	resampledBlobs = contourFinder.blobs;
	for (int i = 0; i < resampledBlobs.size(); i++){
		ofxCvBlob& curBlob = resampledBlobs[i];
		ContourUtils::resampleBlob(curBlob, panel.getValueI("blobSampleRate"), panel.getValueI("blobResampleSpacing"));
	}
	
	// update particles
	Particle::animationBaseFramerate = panel.getValueF("animationBaseFramerate");
	Particle::animationForceFramerate = panel.getValueF("animationForceFramerate");
	Particle::animationVelocityFramerate = panel.getValueF("animationVelocityFramerate");
	Particle::animationScale = panel.getValueF("animationScale");
	Particle::attackScale = panel.getValueF("attackScale");
	Particle::animationDepthScale = panel.getValueF("animationDepthScale");
	Particle::attackRange = panel.getValueF("attackingRange");
	Particle::attackPrecision = panel.getValueF("attackingPrecision");
	Particle::attackDetermination = panel.getValueF("attackingDetermination");
	Particle::attackAccuracy = panel.getValueF("attackingAccuracy");
	Particle::flapDisplacement = panel.getValueF("animationFlapDisplacement");
	Particle::groundForceStart = panel.getValueF("groundForceStart");
	Particle::groundForceAmount = panel.getValueF("groundForceAmount");
	Particle::groundPosition = panel.getValueF("groundPosition");
	Particle::gravity = panel.getValueF("flockingGravity");
	Particle::escapeDistance = panel.getValueF("escapeDistance") * targetHeight / 2;
	Particle::useForward = panel.getValueB("animationUseForward");
	Particle::useFlipping = panel.getValueB("animationUseFlipping");
	Particle::forwardAngle = panel.getValueF("animationForwardAngle");
	
	Chunk::carryDistance = panel.getValueF("chunkCarryDistance");
	Chunk::chunkScale = panel.getValueF("chunkScale");
	
	Particle::setSize(panel.getValueI("flockingSize"), panel.getValueF("flockingStartPosition"));
	Particle::minimumSpeed = panel.getValueF("flockingMinimumSpeed");
	Particle::speed = panel.getValueF("flockingSpeed") * ofGetLastFrameTime() * 256;
	Particle::spread = panel.getValueF("flockingSpread");
	Particle::viscosity = panel.getValueF("flockingViscosity");
	Particle::independence = panel.getValueF("flockingIndependence");
	Particle::neighborhood = panel.getValueF("flockingNeighborhood");
	Particle::turbulence = panel.getValueF("flockingTurbulence") * ofGetLastFrameTime();
	
	if(panel.getValueB("flockingEnable")) {
		Particle::updateAll();
	}
	
	HoleManager::update();
	DebrisManager::update();
	
	panel.clearAllChanged();
}


void testApp::draw(){
	fbo.begin();
	
	ofClear(255, 255, 255, 255);
	
	ofEnableAlphaBlending();
	
	glPushMatrix();
	ofTranslate(targetWidth / 2, targetHeight / 2);
	
	if(panel.getValueB("moveCamera")) {
		ofRotateY(60 * sin(ofMap(mouseX, 0, ofGetWidth(), -HALF_PI, +HALF_PI)));
		ofRotateX(30 * sin(ofMap(mouseY, ofGetHeight(), 0, -HALF_PI, +HALF_PI)));
	}
	
	for (int i = 0; i < contourFinder.nBlobs; i++){
		ofxCvBlob& curBlob = contourFinder.blobs[i];
		ofSetColor(curBlob.hole ? 0 : 255);
		ofFill();
		drawBlob(curBlob);
	}
	
	HoleManager::draw();
	
	if(debug) {
		ofNoFill();
		for (int i = 0; i < contourFinder.nBlobs; i++){
			ofxCvBlob& cur = contourFinder.blobs[i];
			ofSetColor(0);
			drawNormals(cur, 4);
			
			ofSetColor(255, 0, 255);
			cur = resampledBlobs[i];
			drawBlob(cur);
			drawNormals(cur, 8);
		}
		
		ofFill();
		ofPushMatrix();
		ofSetColor(128, 128);
		ofRotateX(-90);
		ofTranslate(-targetWidth / 2, -targetWidth / 2);
		ofPushMatrix();
		ofTranslate(0, 0, Particle::groundForceStart);
		ofRect(0, 0, targetWidth, targetWidth);
		ofPopMatrix();
		ofPushMatrix();
		ofTranslate(0, 0, Particle::groundPosition);
		ofRect(0, 0, targetWidth, targetWidth);
		ofPopMatrix();
		ofPopMatrix();
	}
	
	glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA); // something like darken
	glColor4f(1, 1, 1, 1);
	DebrisManager::draw();
	Particle::drawAnimationAll();
	
	glPopMatrix();
	
	fbo.end();
	
	ofBackground(0, 0, 0);
	ofEnableAlphaBlending();
	ofDisableAlphaBlending();
	
	blur.setRadius(panel.getValueF("blurGlobalRadius"));
	blur.setPasses(panel.getValueF("blurGlobalPasses"));	
	if(!debug) {
		blur.begin();
	}
	glColor4f(1, 1, 1, 1);
	fbo.draw(0, 0);
	if(!debug) {
		blur.end(false);
	}
	
	drawWarped();
}

void testApp::drawWarped() {
	if(debug) {
		fbo.getTexture(0).bind();
	} else {
		blur.getTexture().bind();
	}
	ofClear(0, 0, 0, 1);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(ofGetWidth() * (panel.getValueF("warpNwx") + panel.getValueF("warpNwxFine")), ofGetHeight() * (panel.getValueF("warpNwy") + panel.getValueF("warpNwyFine")));
	glTexCoord2f(targetWidth, 0);
	glVertex2f(ofGetWidth() * (panel.getValueF("warpNex") + panel.getValueF("warpNexFine")), ofGetHeight() * (panel.getValueF("warpNey") + panel.getValueF("warpNeyFine")));
	glTexCoord2f(targetWidth, targetHeight);
	glVertex2f(ofGetWidth() * (panel.getValueF("warpSex") + panel.getValueF("warpSexFine")), ofGetHeight() * (panel.getValueF("warpSey") + panel.getValueF("warpSeyFine")));
	glTexCoord2f(0, targetHeight);
	glVertex2f(ofGetWidth() * (panel.getValueF("warpSwx") + panel.getValueF("warpSwxFine")), ofGetHeight() * (panel.getValueF("warpSwy") + panel.getValueF("warpSwyFine")));
	glEnd();
	if(debug) {
		fbo.getTexture(0).unbind();
	} else {
		blur.getTexture().unbind();
	}
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
			break;
		case 'f':
			panel.setValueB("fullscreen", !panel.getValueB("fullscreen"));
			//ofToggleFullscreen();
			break;	
		case 'a':
			for(int i = 0; i < Particle::particles.size(); i++)
				Particle::particles[i].attackAtRandom();
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
