#include "testApp.h"

void testApp::setup(){
	img.allocate(512, 512, OF_IMAGE_GRAYSCALE);
}

void testApp::update(){
	cv::Mat noise(img.getWidth(), img.getHeight(), CV_8U);
	cv::randn(noise, cv::Scalar::all(128), cv::Scalar::all(20));
	cv::GaussianBlur(noise, noise, cv::Size(3, 3), 0.5, 0.5);
	
	memcpy(img.getPixels(), noise.data, noise.cols * noise.rows);
}

void testApp::draw(){
	img.update();
	img.draw(0, 0);
}
