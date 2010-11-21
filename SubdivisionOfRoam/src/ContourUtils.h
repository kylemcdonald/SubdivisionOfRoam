#pragma once

#include "ofxOpenCv.h"
#include "MSAInterpolator.h"

class ContourUtils {
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
	
	static ofxVec2f closestPoint(const ofxVec2f& p1, const ofxVec2f& p2, const ofxVec2f& p3) {
		float u = (p3.x - p1.x) * (p2.x - p1.x);
		u += (p3.y - p1.y) * (p2.y - p1.y);
		u /= (p2 - p1).length();
		// clamp u
		if(u > 1) {
			u = 1;
		} else if(u < 0) {
			u = 0;
		}
		return p1.interpolated(p2, u);
	}
	
	template <class T>
	static inline T& loopGet(vector<T>& vec, int i) {
		return vec[(i + vec.size()) % vec.size()];
	}
	
	static ofxVec2f closestPoint(ofxCvBlob& blob, const ofxVec2f& target, int& nearest) {
		vector<ofPoint>& pts = blob.pts;
		
		if(pts.size() == 0) {
			return target;
		}
		
		float distance = 0;
		for(int i = 0; i < pts.size(); i++) {
			float curDistance = target.distance(pts[i]);
			if(i == 0 || curDistance < distance) {
				distance = curDistance;
				nearest = i;
			}
		}

		ofxVec2f left = loopGet(pts, nearest - 1);
		ofxVec2f center = pts[nearest];
		ofxVec2f right = loopGet(pts, nearest + 1);
		
		ofxVec2f leftClosest = closestPoint(left, center, target);
		ofxVec2f rightClosest = closestPoint(center, right, target);
		
		if(leftClosest.distance(target) < rightClosest.distance(target)) {
			return leftClosest;
		} else {
			return rightClosest;
		}
	}
	
	static ofxVec2f closestPoint(vector<ofxCvBlob>& blobs, const ofxVec2f& target, ofxCvBlob* matchedBlob, int& matchedIndex) {
		ofxVec2f closest;
		float distance;
		bool hasPoints = false;
		for(int i = 0; i < blobs.size(); i++) {
			ofxCvBlob& cur = blobs[i];
			if(cur.pts.size() > 0) {
				hasPoints = true;
				int curIndex;
				ofxVec2f curClosest = closestPoint(cur, target, curIndex);
				float curDistance = curClosest.distance(target);
				if(i == 0 || curDistance < distance) {
					distance = curDistance;
					closest = curClosest;
					matchedBlob = &cur;
					matchedIndex = curIndex;
				}
			}
		}
		if(hasPoints)
			return closest;
		else
			return target;
	}
	
	static void resampleBlob(ofxCvBlob& blob, int sampleRate, float spacing) {
		vector<ofPoint>& pts = blob.pts;
		vector<ofPoint> resampled;
		
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
		
		/*
		// memo's interpolator is slowing things down...
		// which is why i added sampleRate, so we do more with less here
		
		MSA::Interpolator2D spline;
		spline.setUseLength(true);
		spline.reserve(pts.size());
		int i;
		 
		for(i = 0; i < pts.size(); i += sampleRate) {
			spline.push_back(MSA::Vec2f(pts[i].x, pts[i].y));
		}
		// loop back around if we got to the beginning
		if(pts.size() > 0 && i == pts.size()) {
			spline.push_back(MSA::Vec2f(pts[0].x, pts[0].y));
		}
		for(i = 0; i < spacing; i++) {
			MSA::Vec2f interpolated = spline.sampleAt((float) i / spacing);
			resampled.push_back(ofPoint(interpolated.x, interpolated.y));
		}
		*/
		
		pts = resampled;
	}
	
	static void getOffsetPoint(ofxCvBlob& blob, int start, float radius, int& lastIndex, ofPoint& point) {
		/*
		 vector<ofPoint>& pts = blob.pts;
		if(radius > 0) {
			float curRadius = 0;
			lastIndex = start;
			// find the first index after the radius
			do {
				ofPoint& lastPoint = loopGet(pts, lastIndex);
				ofPoint& curPoint = loopGet(pts, lastIndex++);
				curRadius += ofDist(lastPoint.x, lastPoint.y, curPoint.x, curPoint.y);
			} while(curRadius < radius);
			// overshoots once, bring back
			lastIndex--;
			ofPoint& lastPoint = loopGet(pts, lastIndex);
			ofPoint& curPoint = loopGet(pts, lastIndex + 1);
			curRadius -= ofDist(lastPoint.x, lastPoint.y, curPoint.x, curPoint.y);
			// then interpolate
		} else {
		}
		 */
	}
	
	static void getCenteredSegment(ofxCvBlob& blob, int center, float radius, int& startIndex, int& stopIndex, ofxVec2f& firstPoint, ofxVec2f& lastPoint) {
		//getOffsetPoint(blob, center, -radius, startIndex, firstPoint);
		//getOffsetPoint(blob, center, +radius, stopIndex, lastPoint);
	}
};