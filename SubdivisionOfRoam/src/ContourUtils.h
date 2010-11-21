#pragma once

#include "ofMath.h"
#include "ofxOpenCv.h"
#include "MSAInterpolator.h"
#include "ofxVectorMath.h"

class ContourUtils {
public:
	static void smoothBlobs(vector<ofxCvBlob>& blobs, int smoothingSize, float smoothingAmount);
	static void smoothBlob(ofxCvBlob& blob, int smoothingSize, float smoothingAmount);
	static ofxVec2f closestPoint(const ofxVec2f& p1, const ofxVec2f& p2, const ofxVec2f& p3);
	static ofxVec2f closestPoint(ofxCvBlob& blob, const ofxVec2f& target, int& nearest);
	static ofxVec2f closestPoint(vector<ofxCvBlob>& blobs, const ofxVec2f& target, ofxCvBlob*& matchedBlob, int& matchedIndex);	
	static void resampleBlob(ofxCvBlob& blob, int sampleRate, float spacing);
	static void getOffsetPoint(ofxCvBlob& blob, int start, float radius, int& lastIndex, ofxVec2f& point);
	static int loopMod(int i, int n);
	template <class T> static T& loopGet(vector<T>& vec, int i);
	static void getCenteredSegment(ofxCvBlob& blob, int center, float radius, int& startIndex, int& stopIndex, ofxVec2f& firstPoint, ofxVec2f& lastPoint);
};