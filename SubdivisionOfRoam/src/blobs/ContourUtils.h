#pragma once

#include "ofMath.h"
#include "ofxOpenCv.h"
#include "MSAInterpolator.h"
#include "ofxVectorMath.h"

class ContourUtils {
public:
	// given a blob or a vector of blobs, smooth all the points using a linear dropoff kernel
	// of radius smoothingSize and weighted towards the edges with smoothingAmount
	// smoothingAmount of 1 will give a moving average, while 0 gives a triangle kernel
	static void smoothBlobs(vector<ofxCvBlob>& blobs, int smoothingSize, float smoothingAmount);
	static void smoothBlob(ofxCvBlob& blob, int smoothingSize, float smoothingAmount);
	
	// find the closest point to p3 on the line between p1 and p2
	// optionally pass a pointer to a float for the normalized position along that line
	// also, helper functions for finding the closest point on a blob or closest in a vector of blobs
	static ofxVec2f closestPoint(const ofxVec2f& p1, const ofxVec2f& p2, const ofxVec2f& p3, float* uptr = NULL);
	static ofxVec2f closestPoint(ofxCvBlob& blob, const ofxVec2f& target, int& nearest);
	static ofxVec2f closestPoint(vector<ofxCvBlob>& blobs, const ofxVec2f& target, ofxCvBlob*& matchedBlob, int& matchedIndex);	
	
	// evenly resample a blob with equally spaced points. best to do this after smoothing.
	static void resampleBlob(ofxCvBlob& blob, int sampleRate, float spacing);
	
	// get a point on a blob that is a distance in pixels from a given point
	static void getOffsetPoint(ofxCvBlob& blob, int start, float radius, int& lastIndex, ofxVec2f& point);
	
	// return two points that lay on a blob an equal distance from a given point
	static void getCenteredSegment(ofxCvBlob& blob, int center, float radius, int& startIndex, int& stopIndex, ofxVec2f& firstPoint, ofxVec2f& lastPoint);

	// helper functions for accessing points in blobs without worrying about wraparound indexing
	static int loopMod(int i, int n);
	template <class T> static T& loopGet(vector<T>& vec, int i);
};