#include "ofMain.h"
#include "ofApp.h"


//========================================================================
int main( ){

	ofSetLogLevel(OF_LOG_VERBOSE);

//#ifdef TARGET_OPENGLES
//	//ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLProgrammableRenderer()));
//	ofGLESWindowSettings settings;
//    settings.width = 800;
//    settings.height = 480;
//    settings.setGLESVersion(2);	
//    ofCreateWindow(settings);
//
//    ofRunApp(new ofApp());
//#else
	ofSetupOpenGL(800,480,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	ofRunApp(new ofApp());

//#endif
	
}

