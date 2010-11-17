#pragma once

#include "ofxOpenCv.h"
#include "MSAInterpolator.h"

class ContourMatcher {
public:
	static void smoothBlobs(vector<ofxCvBlob>& blobs, int smoothingSize, float smoothingAmount) {
		for(int i = 0; i < blobs.size(); i++) {
			smoothBlob(blobs[i], smoothingSize, smoothingAmount);
		}
	}
	
	static void smoothBlob(ofxCvBlob& blob, int smoothingSize, float smoothingAmount) {
		vector<ofPoint> ref = blob.pts;
		vector<ofPoint>& cur = blob.pts;
		// should precompute weightSum, weights, and total normalization]
		int n = cur.size();
		for(int i = 0; i < n; i++) {
			cur[i].set(0, 0);
			float weightSum = 0;
			for(int j = 1; j <= smoothingSize; j++) {
				int leftPosition = (n + i - j) % n;
				int rightPosition = (i + j) % n;
				ofPoint& left = ref[leftPosition];
				ofPoint& right = ref[rightPosition];
				
				float weight = ofMap(j, 0, smoothingSize, 1, smoothingAmount);
				weightSum += weight;
				cur[i] += (left + right) * weight;
			}
			ofPoint& center = ref[i];
			cur[i] += center;
			cur[i] /= 1 + 2 * weightSum;
		}
		
		// does not update the centroid
		// or the bounding box
	}
	
	static void resampleBlob(ofxCvBlob& blob, float spacing) {
		vector<ofPoint>& pts = blob.pts;
		vector<ofPoint> resampled;
		
		/*
		// alternate technique for evenly spaced samples
		float totalLength = 0;
		int curStep = 0;
		for(int i = 0; i < pts.size() - 1; i++) {
			ofxVec2f cur = pts[i];
			ofxVec2f next = pts[i + 1];
			ofxVec2f diff = next - cur;
			
			float curSegmentLength = diff.length();
			totalLength += curSegmentLength;
			
			while(curStep * spacing <= totalLength) {
				float curSample = curStep * spacing;
				float curLength = curSample - (totalLength - curSegmentLength);
				float relativeSample = curLength / curSegmentLength;
				resampled.push_back(cur.interpolate(next, relativeSample));
				curStep++;
			}
		}
		*/
		
		MSA::Interpolator2D spline;
		spline.setUseLength(true);
		spline.reserve(pts.size());
		for(int i = 0; i < pts.size(); i++) {
			spline.push_back(MSA::Vec2f(pts[i].x, pts[i].y));
		}
		// loop back around
		if(pts.size() > 0) {
			spline.push_back(MSA::Vec2f(pts[0].x, pts[0].y));
		}
		for(int i = 0; i < spacing; i++) {
			MSA::Vec2f interpolated = spline.sampleAt((float) i / spacing);
			resampled.push_back(ofPoint(interpolated.x, interpolated.y));
		}
		
		pts = resampled;
	}
};