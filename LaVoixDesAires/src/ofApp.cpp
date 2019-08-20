#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {


	//FINAL DIMENSION - FINAL DIMENSION - FINAL DIMENSION
    final_w = 2048;
    final_h = 768;
    //FINAL DIMENSION - FINAL DIMENSION - FINAL DIMENSION
    
    
    ofSetFrameRate(60);

	// Background Image, ofPolyline ...
	polyBackground = new PolyBackground(&pg_polyBackground, 2048, 768);

	// OSC, Gui parameters
	pg_birdManager.setName("birdmanager");

	//BirdManager
	birdManager = new BirdManager(polyBackground, &pg_birdManager, final_w, final_h);

    // Text Manager
    textManager = new TextManager(birdManager, &pg_textManager);
    
    //Global parameter
    pg.setName("main");
    pg.add(color.set("color",200,0,255));
    pg.add(frameRate.set("frameRate", 30, 0, 50));
    pg.add(debug.set("debug", false));
    pg.add(fakeCursor.set("Fake cursor", false));
    
    // Adding all parameter to GUI
    gui.setup(pg);
    gui.add(pg_birdManager);
    gui.add(pg_polyBackground);
    gui.add(pg_textManager);
    sync.setup((ofParameterGroup&)gui.getParameter(), 6666, "localhost", 6667);

	//3D stuff
	ofDisableArbTex();

	//fbo allocation
	fbo.allocate(final_w, final_h, GL_RGBA);

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
    
    // Polybackground - draw center of point as a circle
    polyBackground->draw();
    //Polybackground - draw the pencil mode
    if(polyBackground->isPencil){
        
        polyBackground->pencilOnFbo();
        ofSetColor(255);
        polyBackground->fbo.draw(0,0);
        
    }
    
    // Fake cursor
    if(fakeCursor){
        
        ofVec2f cursor = ofVec2f(ofGetMouseX() * (final_w*1.0) /ofGetWidth() , ofGetMouseY() * (final_h*1.0)/ofGetHeight());
        
        ofSetColor(255, 0 ,0);
        ofDrawCircle( cursor, 20);
        ofDrawLine(cursor.x - 30, cursor.y, cursor.x + 30, cursor.y);
        ofDrawLine(cursor.x , cursor.y- 30, cursor.x , cursor.y + 30);
        
        
    }
    
	fbo.end();

	//DEBUG PART
	if (debug) {
		ofDrawBitmapStringHighlight("FrameRate : " + ofToString(ofGetFrameRate()), ofGetWidth() / 2, ofGetHeight());
	}
    

    //FBO
    ofSetColor(255);
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    //GUI
    gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

    if(key != 8){
       textManager->addLetter(key);
    }
    else{
        textManager->clear();
    }
    
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
