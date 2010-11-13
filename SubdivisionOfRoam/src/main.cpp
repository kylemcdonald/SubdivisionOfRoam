#include "testApp.h"

int main() {
	ofAppGlutWindow window;
	// specify width, height and OF_WINDOW or OF_FULLSCREEN
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp(new testApp());
}
