#include "testApp.h"

int main() {
	ofAppGlutWindow window;
	// specify width, height and OF_WINDOW or OF_FULLSCREEN
	ofSetupOpenGL(&window, 540 + 300, 960, OF_WINDOW);
	ofRunApp(new testApp());
}
