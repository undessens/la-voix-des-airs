#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {


	ofSetFrameRate(60);

	// Background Image, ofPolyline ...
	polyBackground = new PolyBackground(&pg_polyBackground);

	// OSC, Gui parameters
	pg_birdManager.setName("birdmanager");

	//BirdManager
	birdManager = new BirdManager(polyBackground, &pg_birdManager);

    // Text Manager
    textManager = new TextManager(birdManager, &pg_textManager);
    
    //Global parameter
    pg.setName("main");
    pg.add(color.set("color",200,0,255));
    pg.add(frameRate.set("frameRate", 30, 0, 50));
    pg.add(debug.set("debug", false));
    
    // Adding all parameter to GUI
    gui.setup(pg);
    gui.add(pg_birdManager);
    gui.add(pg_polyBackground);
    gui.add(pg_textManager);
    sync.setup((ofParameterGroup&)gui.getParameter(), 6666, "localhost", 6667);

	//3D stuff
	ofDisableArbTex();

	//fbo allocation
	fbo.allocate(1024, 768, GL_RGBA);

	// OSC receiver
	osc_receiver.setup(12345); 



	// sender name for madmapper
#if defined(_WIN32)
	sender.init("My_Lovely_Birds");
#endif
    
}

//--------------------------------------------------------------
void ofApp::update() {

	birdManager->update();

	// GUI update
	sync.update();

    //SPOUT windows only
#if defined(_WIN32)
    sender.send(fbo.getTexture());
#endif
	
	while (osc_receiver.hasWaitingMessages()) {
		ofxOscMessage m; 
		osc_receiver.getNextMessage(m);
		if (m.getAddress() == "/debug") {
			letter = m.getArgAsInt32(0);
			ofLog() << "message : " << letter; 
			textManager->addLetter(letter);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	fbo.begin();

	ofBackground(color);
	ofSetVerticalSync(true);

	// LIGHT MANAGEMENT
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableDepthTest();
#ifndef TARGET_PROGRAMMABLE_GL
	glShadeModel(GL_SMOOTH);
#endif // !TARGET_PROGRAMMABLE_GL
	light.enable();

	// BIRD MANAGER
	birdManager->draw();

	ofDisableDepthTest();
	light.disable();
	ofDisableLighting();
	ofDisableSeparateSpecularLight();

	// Text Manager
    textManager->drawPoly();
	fbo.end();

	//DEBUG PART
	if (debug) {
		ofDrawBitmapStringHighlight("FrameRate : " + ofToString(ofGetFrameRate()), ofGetWidth() / 2, ofGetHeight());
	}
    
    //FBO
    ofSetColor(255);
	fbo.draw(0, 0);
	//GUI
	
    
    gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	textManager->addLetter(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
