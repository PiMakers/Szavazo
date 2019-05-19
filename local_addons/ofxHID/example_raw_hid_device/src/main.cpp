//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//

/*
#include "ofApp.h"


int main()
{
    ofSetupOpenGL(640, 480, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
*/

#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofApp.h"

//========================================================================
int main( ){
	auto window = make_shared<ofAppNoWindow>();
	auto app = make_shared<ofApp>();
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(window, app);
	ofRunMainLoop();
}